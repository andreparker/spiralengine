#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <cstdlib>
#include <boost/thread/mutex.hpp>
#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include "Singleton.ipp"
#include "File.hpp"

#define POLICY typename MemoryPolicy, boost::int32_t MemoryBudget
#define TEMPL_MEMORYPOLICY template< POLICY >

namespace Spiral
{
	template< boost::int32_t number >
	struct KiloByte
	{
		static const boost::uint32_t value = number << 10;
	};

	template< boost::int32_t number >
	struct MegaByte
	{
		static const boost::uint32_t value = KiloByte< number << 10 >::value;
	};

	template< POLICY >
	class MemoryManager : public Singleton< MemoryManager<MemoryPolicy,MemoryBudget> >
	{
		DECL_SINGLETON( MemoryManager )
	public:

		~MemoryManager();
		MemoryManager();

		/*!
		   @function  Initialize
		   @brief     initializes the memory manager with MemoryBudget
		   @return    bool - if base memory could not be allocated
		*/
		bool Initialize();

		void UnInitialize();

		/*!
		   @function  Alloc
		   @brief     allocates num bytes from the heap
		   @return    void* - null on failure
		   @param     boost::uint32_t bytes
		*/
		void* Alloc( boost::uint32_t bytes );

		/*!
		   @function  Alloc
		   @brief     allocates num bytes and records were the memory was requested
		   @return    void* - null on failure
		   @param     boost::uint32_t bytes - the amount of memory to request
		   @param     const char * srcFileName - source c/c++ file name
		   @param     boost::int32_t lineNumber - the line number in the file
		*/
		void* Alloc( boost::uint32_t bytes, const char* srcFileName, boost::int32_t lineNumber );

		/*!
		   @function  Dealloc
		   @brief     frees memory
		   @return    void
		   @param     void * ptr
		*/
		void Dealloc( void* ptr );

		/*!
		   @function  WriteAnalysis
		   @brief     writes out memory data
		   @return    void
		   @param     const boost::shared_ptr<IFile> & file
		*/
		void WriteAnalysis( const boost::shared_ptr<OFile>& file )const;

		bool SetAllocInfo( const char* fileName, boost::int32_t lineNumber );

		boost::uint32_t BytesAllocated()const
		{
			return m_totalAllocatedSize;
		}

		boost::uint32_t TotalAllocations()const
		{
			return m_totalAllocations;
		}

	private:
		
		struct MemoryAllocBlock
		{
			boost::uint32_t managerId;
			const char* srcFileName;
			boost::int32_t lineNumber;
			boost::uint32_t bytes;
			MemoryAllocBlock* next;
		};

		const char* UnKnownSrcFile;
		static const boost::uint32_t MemoryBlockSize = sizeof( MemoryAllocBlock );

		MemoryPolicy m_memoryPolicyClass;	// class in charge of allocations
		MemoryAllocBlock *m_headBlock,*m_tailBlock;
		boost::uint32_t m_totalAllocatedSize;
		boost::uint32_t m_totalAllocations;
		boost::uint32_t m_managerId;
		boost::mutex m_mutex;
	private:
		const char* m_srcFileName;
		boost::int32_t m_lineNumber;

		void AddBlock( MemoryAllocBlock* block );
		void RemoveBlock( const MemoryAllocBlock* block );
	};


	TEMPL_MEMORYPOLICY MemoryManager<MemoryPolicy,MemoryBudget>::MemoryManager():
	UnKnownSrcFile("UnKnown Src"),
	m_memoryPolicyClass(),
	m_headBlock( NULL ),
	m_tailBlock( NULL ),
	m_totalAllocatedSize( 0 ),
	m_totalAllocations( 0 ),
	m_mutex()
	{
		Initialize();
		m_managerId = rand();
	}

	TEMPL_MEMORYPOLICY MemoryManager<MemoryPolicy,MemoryBudget>::~MemoryManager()
	{
		if( m_headBlock )
		{
			UnInitialize();
		}
	}

	TEMPL_MEMORYPOLICY bool MemoryManager<MemoryPolicy,MemoryBudget>::Initialize()
	{
		return m_memoryPolicyClass.Initialize( MemoryBudget );
	}

	TEMPL_MEMORYPOLICY void MemoryManager<MemoryPolicy,MemoryBudget>::UnInitialize()
	{
		if( m_headBlock )
		{
			MemoryAllocBlock* itr = m_headBlock;
			MemoryAllocBlock* NextItr = m_headBlock;
			while( itr )
			{
				NextItr = itr->next;
				m_memoryPolicyClass.Dealloc( itr );
				itr = NextItr;
			}
		}
		m_memoryPolicyClass.UnInitialize();
	}

	TEMPL_MEMORYPOLICY bool MemoryManager<MemoryPolicy,MemoryBudget>::SetAllocInfo( const char* fileName, boost::int32_t lineNumber )
	{
		m_srcFileName = fileName;
		m_lineNumber = lineNumber;
		return false;		// returns false for macro use
	}

	TEMPL_MEMORYPOLICY void* MemoryManager<MemoryPolicy,MemoryBudget>::Alloc(boost::uint32_t bytes)
	{
		return Alloc( bytes, m_srcFileName, m_lineNumber );
	}

	TEMPL_MEMORYPOLICY void* MemoryManager<MemoryPolicy,MemoryBudget>::Alloc( boost::uint32_t bytes, const char* srcFileName, boost::int32_t lineNumber )
	{
		if( m_mutex.try_lock() )
		{
			void* ptr = m_memoryPolicyClass.Alloc( MemoryBlockSize + bytes );
			if( ptr != NULL )
			{
				MemoryAllocBlock* block = static_cast<MemoryAllocBlock*>( ptr );
				block->managerId = m_managerId;
				block->bytes = bytes;
				block->lineNumber = lineNumber;
				block->srcFileName = srcFileName;
				block->next = NULL;

				//AddBlock( block );

				ptr = ( ((char*)ptr) + MemoryBlockSize );
			}
			m_mutex.unlock();
			return ptr;
		}

		return NULL;
	}

	TEMPL_MEMORYPOLICY void MemoryManager<MemoryPolicy,MemoryBudget>::Dealloc( void* ptr )
	{
		if( ptr )
		{
			while( m_mutex.try_lock() == false ){}

			ptr = (((char*)ptr) - MemoryBlockSize );
			//RemoveBlock( static_cast<MemoryAllocBlock*>( ptr ) );
			m_memoryPolicyClass.Dealloc( ptr );

			m_mutex.unlock();
		}
	}

	TEMPL_MEMORYPOLICY void MemoryManager<MemoryPolicy,MemoryBudget>::WriteAnalysis( const boost::shared_ptr<OFile>& file )const
	{
		shared_ptr< std::ostream > streamPtr = file->GetStream();
		std::ostream* stream = streamPtr.get();

		*stream << "----------Memory Manager Analysis----------" << std::endl;
		*stream << "Total memory leaked: " << m_totalAllocatedSize << std::endl;
		*stream << "Total allocations leaked: " << m_totalAllocations << std::endl;

		if( m_headBlock )
		{
			*stream << "----------Leaked Pointer list----------" << std::endl;
			boost::int32_t pointerCount = 0;
			for( MemoryAllocBlock* itr = m_headBlock; itr != NULL; itr = itr->next, ++pointerCount )
			{
				*stream << "Allocation - " << pointerCount << std::endl;
				*stream << "   Source File: " << itr->srcFileName << std::endl;
				*stream << "   Line Number: " << itr->lineNumber << std::endl;
				*stream << "   Bytes : " << itr->bytes << std::endl;
			}
			*stream << "----------Leaked Pointer End----------" << std::endl;
		}

		file->WriteOutStream( streamPtr );
	}

	TEMPL_MEMORYPOLICY void MemoryManager<MemoryPolicy,MemoryBudget>::AddBlock( typename MemoryManager<MemoryPolicy,MemoryBudget>::MemoryAllocBlock* block )
	{
		if( m_headBlock == NULL )
		{
			m_headBlock = block;
			m_tailBlock = block;
		}else
		{
			m_tailBlock->next = block;
			m_tailBlock = m_tailBlock->next;
			
		}

		// record the memory size
		m_totalAllocatedSize += block->bytes;
		++m_totalAllocations;
	}

	TEMPL_MEMORYPOLICY void MemoryManager<MemoryPolicy,MemoryBudget>::RemoveBlock( const typename MemoryManager<MemoryPolicy,MemoryBudget>::MemoryAllocBlock* block )
	{
		MemoryAllocBlock* itr = m_headBlock;
		const MemoryAllocBlock* endItr = NULL;

		BOOST_ASSERT( block->managerId == m_managerId );

		if( m_headBlock == block )
		{
			if( m_tailBlock == m_headBlock )
			{
				m_tailBlock = NULL;
			}

			m_headBlock = m_headBlock->next;	
		}else
		{
			MemoryAllocBlock* last = itr;
			for( itr = itr->next; itr != endItr; itr = itr->next )
			{
				if( itr == block )
				{
					last->next = itr->next;
					break;
				}

				last = itr;
			}
		}

		--m_totalAllocations;
		m_totalAllocatedSize -= block->bytes;
	}

//#undef new
//#define new MemoryManager< MallocPolicy, 0 >::instance().SetAllocInfo( __FILE__, __LINE__ ) ? NULL : new

}

#endif