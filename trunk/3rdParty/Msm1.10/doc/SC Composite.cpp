#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include "boost/mpl/list.hpp"

#include <vector>
#include "boost/mpl/vector/vector50.hpp"
#include <boost/msm/state_machine.hpp>

#include <iostream>
#ifdef WIN32
#include "windows.h"
#else
#include <sys/time.h>
#endif

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

namespace test_sc
{

    //events
    struct play : sc::event< play > {};
    struct end_pause : sc::event< end_pause > {};
    struct stop : sc::event< stop > {};
    struct pause : sc::event< pause > {};
    struct open_close : sc::event< open_close > {};
    struct cd_detected : sc::event< cd_detected > {};
    struct NextSong: sc::event< NextSong > {};
    struct PreviousSong : sc::event< PreviousSong >{};

    struct Empty;
    struct Open;
    struct Stopped;
    struct Playing;
    struct Paused;
    // SM
    struct player : sc::state_machine< player, Empty > 
    {
        void open_drawer(open_close const&)         { /*std::cout << "player::open_drawer\n";*/ }
        void store_cd_info(cd_detected const& cd)   {/*std::cout << "player::store_cd_info\n";*/ }
        void close_drawer(open_close const&)        { /*std::cout << "player::close_drawer\n";*/ }
        void start_playback(play const&)            { /*std::cout << "player::start_playback\n";*/ }
        void stopped_again(stop const&)	            {/*std::cout << "player::stopped_again\n";*/}
        void stop_playback(stop const&)             { /*std::cout << "player::stop_playback\n";*/ }
        void pause_playback(pause const&)           { /*std::cout << "player::pause_playback\n"; */}
        void stop_and_open(open_close const&)       { /*std::cout << "player::stop_and_open\n";*/ }
        void resume_playback(end_pause const&)      { /*std::cout << "player::resume_playback\n";*/ }
    };

    struct Empty : sc::simple_state< Empty, player >
    {
        Empty() { /*std::cout << "entering Empty" << std::endl;*/ } // entry
        ~Empty() { /*std::cout << "leaving Empty" << std::endl;*/ } // exit
        typedef mpl::list<
            sc::transition< open_close, Open,
            player, &player::open_drawer >,
            sc::transition< cd_detected, Stopped,
            player, &player::store_cd_info > > reactions;

    };
    struct Open : sc::simple_state< Open, player >
    {
        Open() { /*std::cout << "entering Open" << std::endl;*/ } // entry
        ~Open() { /*std::cout << "leaving Open" << std::endl;*/ } // exit
        typedef sc::transition< open_close, Empty,
            player, &player::close_drawer > reactions;

    };
    struct Stopped : sc::simple_state< Stopped, player >
    {
        Stopped() { /*std::cout << "entering Stopped" << std::endl;*/ } // entry
        ~Stopped() { /*std::cout << "leaving Stopped" << std::endl;*/ } // exit
        typedef mpl::list<
            sc::transition< play, Playing,
            player, &player::start_playback >,
            sc::transition< open_close, Open,
            player, &player::open_drawer >, 
            sc::transition< stop, Stopped,
            player, &player::stopped_again > > reactions;

    };
    struct Song1;
    struct Playing : sc::simple_state< Playing, player,Song1 >
    {
        Playing() { /*std::cout << "entering Playing" << std::endl;*/ } // entry
        ~Playing() { /*std::cout << "leaving Playing" << std::endl;*/ } // exit
        typedef mpl::list<
            sc::transition< stop, Stopped,
            player, &player::stop_playback >,
            sc::transition< pause, Paused,
            player, &player::pause_playback >, 
            sc::transition< open_close, Open,
            player, &player::stop_and_open > > reactions;
        void start_next_song(NextSong const&)       { /*std::cout << "Playing::start_next_song\n";*/ }
        void start_prev_song(PreviousSong const&)       { /*std::cout << "Playing::start_prev_song\n";*/ }
    };
    struct Song2;
    struct Song1  : sc::simple_state< Song1, Playing >
    {
        Song1() { /*std::cout << "entering Song1" << std::endl;*/ } // entry
        ~Song1() { /*std::cout << "leaving Song1" << std::endl;*/ } // exit
        typedef sc::transition< NextSong, Song2,
            Playing, &Playing::start_next_song > reactions;
    };
    struct Song3;
    struct Song2  : sc::simple_state< Song2, Playing >
    {
        Song2() { /*std::cout << "entering Song2" << std::endl;*/ } // entry
        ~Song2() { /*std::cout << "leaving Song2" << std::endl;*/ } // exit
        typedef mpl::list<
            sc::transition< NextSong, Song3,
            Playing, &Playing::start_next_song >,
            sc::transition< PreviousSong, Song1,
            Playing, &Playing::start_prev_song > > reactions;
    };
    struct Song3  : sc::simple_state< Song3, Playing >
    {
        Song3() { /*std::cout << "entering Song3" << std::endl;*/ } // entry
        ~Song3() { /*std::cout << "leaving Song3" << std::endl;*/ } // exit
        typedef sc::transition< PreviousSong, Song2,
            Playing, &Playing::start_prev_song > reactions;
    };
    struct Paused : sc::simple_state< Paused, player >
    {
        Paused() { /*std::cout << "entering Paused" << std::endl;*/ } // entry
        ~Paused() { /*std::cout << "leaving Paused" << std::endl;*/ } // exit
        typedef mpl::list<
            sc::transition< end_pause, Playing,
            player, &player::resume_playback >,
            sc::transition< stop, Stopped,
            player, &player::stop_playback >, 
            sc::transition< open_close, Open,
            player, &player::stop_and_open > > reactions;
    };
}

//FSM

using namespace boost::msm;

namespace test_fsm // Concrete FSM implementation
{
    // events
    struct play {};
    struct end_pause {};
    struct stop {};
    struct pause {};
    struct open_close {};
    struct cd_detected{};
    struct NextSong {};
    struct PreviousSong {};


    // Concrete FSM implementation 
    struct player : public state_machine<player>
    {
        // The list of FSM states
        struct Empty : public state<> 
        {
            // optional entry/exit methods
            template <class Event>
            void on_entry(Event const& ) {/*std::cout << "entering: Empty" << std::endl;*/}
            template <class Event>
            void on_exit(Event const& ) {/*std::cout << "leaving: Empty" << std::endl;*/}
        };
        struct Open : public state<> 
        {	 
            template <class Event>
            void on_entry(Event const& ) {/*std::cout << "entering: Open" << std::endl;*/}
            template <class Event>
            void on_exit(Event const& ) {/*std::cout << "leaving: Open" << std::endl;*/}
        };

        struct Stopped : public state<> 
        {	 
            // when stopped, the CD is loaded
            template <class Event>
            void on_entry(Event const& ) {/*std::cout << "entering: Stopped" << std::endl;*/}
            template <class Event>
            void on_exit(Event const& ) {/*std::cout << "leaving: Stopped" << std::endl;*/}
        };

        struct Playing : public state_machine<Playing>
        {
            // when playing, the CD is loaded and we are in either pause or playing (duh)
            

            // The list of FSM states
            struct Song1 : public state<>
            {  };
            struct Song2 : public state<>
            {	 };
            struct Song3 : public state<>
            {	 };
            // the initial state. Must be defined
            typedef Song1 initial_state;
            // transition actions
            void start_next_song(NextSong const&)       { /*std::cout << "Playing::start_next_song\n";*/ }
            void start_prev_song(PreviousSong const&)       { /*std::cout << "Playing::start_prev_song\n"; */}
            // guard conditions

            // friend definition needed.
            friend class state_machine<Playing>;
            typedef Playing pl; // makes transition table cleaner
            // Transition table for Playing
            struct transition_table : mpl::vector4<
                //    Start     Event         Next      Action				Guard
                //    +---------+-------------+---------+---------------------+----------------------+
                a_row < Song1   , NextSong    , Song2   , &pl::start_next_song                     >,
                a_row < Song2   , PreviousSong, Song1   , &pl::start_prev_song                     >,
                a_row < Song2   , NextSong    , Song3   , &pl::start_next_song                     >,
                a_row < Song3   , PreviousSong, Song2   , &pl::start_prev_song                     >
                //    +---------+-------------+---------+---------------------+----------------------+
            > {};
            // Replaces the default no-transition response.
            template <class Event>
            int no_transition(int state, Event const& e)
            {
                std::cout << "no transition from state " << state
                    << " on event " << typeid(e).name() << std::endl;
                return state;
            }
        };

        // state not defining any entry or exit
        struct Paused : public state<>
        {
            template <class Event>
            void on_entry(Event const& ) {/*std::cout << "entering: Paused" << std::endl;*/}
            template <class Event>
            void on_exit(Event const& ) {/*std::cout << "leaving: Paused" << std::endl;*/}
        };

        // the initial state of the player SM. Must be defined
        typedef Empty initial_state;

#ifdef __MWERKS__
    public: // Codewarrior bug workaround.  Tested at 0x3202
#endif 
        // transition actions
        void start_playback(play const&)       {  }
        void open_drawer(open_close const&)    {  }
        void close_drawer(open_close const&)   {  }
        void store_cd_info(cd_detected const& cd) { }
        void stop_playback(stop const&)        {  }
        void pause_playback(pause const&)      { }
        void resume_playback(end_pause const&)      {  }
        void stop_and_open(open_close const&)  {  }
        void stopped_again(stop const&)	{}
        // guard conditions

#ifdef __MWERKS__
    private:
#endif 
        // friend definition needed.
        friend class state_machine<player>;
        typedef player p; // makes transition table cleaner

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start     Event         Next      Action				Guard
            //    +---------+-------------+---------+---------------------+----------------------+
            a_row < Stopped , play        , Playing , &p::start_playback                       >,
            a_row < Stopped , open_close  , Open    , &p::open_drawer                          >,
            a_row < Stopped , stop        , Stopped , &p::stopped_again                        >,
            //    +---------+-------------+---------+---------------------+----------------------+
            a_row < Open    , open_close  , Empty   , &p::close_drawer                         >,
            //    +---------+-------------+---------+---------------------+----------------------+
            a_row < Empty   , open_close  , Open    , &p::open_drawer                          >,
            a_row < Empty   , cd_detected , Stopped , &p::store_cd_info                        >,
            //    +---------+-------------+---------+---------------------+----------------------+
            a_row < Playing , stop        , Stopped , &p::stop_playback                        >,
            a_row < Playing , pause       , Paused  , &p::pause_playback                       >,
            a_row < Playing , open_close  , Open    , &p::stop_and_open                        >,
            //    +---------+-------------+---------+---------------------+----------------------+
            a_row < Paused  , end_pause   , Playing , &p::resume_playback                      >,
            a_row < Paused  , stop        , Stopped , &p::stop_playback                        >,
            a_row < Paused  , open_close  , Open    , &p::stop_and_open                        >
            //    +---------+-------------+---------+---------------------+----------------------+
        > {};

        // Replaces the default no-transition response.
        template <class Event>
        int no_transition(int state, Event const& e)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
            return state;
        }
    };

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "Stopped", "Open", "Empty", "Playing", "Paused" };

    void pstate(player const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

}


#ifndef WIN32
long mtime(struct timeval& tv1,struct timeval& tv2)
{
    return (tv2.tv_sec-tv1.tv_sec) *1000000 + ((tv2.tv_usec-tv1.tv_usec));
}
#endif

int main()
{
    test_sc::player p;
    p.initiate();
    // for timing
#ifdef WIN32
    LARGE_INTEGER res;
    ::QueryPerformanceFrequency(&res);
    LARGE_INTEGER li,li2;
    ::QueryPerformanceCounter(&li);
#else
    struct timeval tv1,tv2;
    gettimeofday(&tv1,NULL);
#endif

    for (int i=0;i<100;++i)
    {
        p.process_event(test_sc::open_close());
        p.process_event(test_sc::open_close()); 
        p.process_event(test_sc::cd_detected());
        p.process_event(test_sc::play()); 
        for (int j=0;j<100;++j)
        {
            p.process_event(test_sc::NextSong());
            p.process_event(test_sc::NextSong());
            p.process_event(test_sc::PreviousSong());
            p.process_event(test_sc::PreviousSong());
        }

        p.process_event(test_sc::pause()); 
        // go back to Playing
        p.process_event(test_sc::end_pause()); 
        p.process_event(test_sc::pause()); 
        p.process_event(test_sc::stop());  
        // event leading to the same state
        p.process_event(test_sc::stop());
        p.process_event(test_sc::open_close());
        p.process_event(test_sc::open_close());
    }
#ifdef WIN32
    ::QueryPerformanceCounter(&li2);
#else
    gettimeofday(&tv2,NULL);
#endif
#ifdef WIN32
    std::cout << "sc took in s:" << (double)(li2.QuadPart-li.QuadPart)/res.QuadPart <<"\n" <<std::endl;
#else
    std::cout << "sc took in us:" << mtime(tv1,tv2) <<"\n" <<std::endl;
#endif

    test_fsm::player p2;
    p2.start();
    // for timing
#ifdef WIN32
    ::QueryPerformanceCounter(&li);
#else
    gettimeofday(&tv1,NULL);
#endif
    for (int i=0;i<100;++i)
    {
        p2.process_event(test_fsm::open_close());
        p2.process_event(test_fsm::open_close()); 
        p2.process_event(test_fsm::cd_detected());
        p2.process_event(test_fsm::play());
        for (int j=0;j<100;++j)
        {
            p2.process_event(test_fsm::NextSong());
            p2.process_event(test_fsm::NextSong());
            p2.process_event(test_fsm::PreviousSong());
            p2.process_event(test_fsm::PreviousSong());
        }

        p2.process_event(test_fsm::pause()); 
        // go back to Playing
        p2.process_event(test_fsm::end_pause()); 
        p2.process_event(test_fsm::pause()); 
        p2.process_event(test_fsm::stop());  
        // event leading to the same state
        p2.process_event(test_fsm::stop());
        p2.process_event(test_fsm::open_close());
        p2.process_event(test_fsm::open_close());
    }
#ifdef WIN32
    ::QueryPerformanceCounter(&li2);
#else
    gettimeofday(&tv2,NULL);
#endif
#ifdef WIN32
    std::cout << "msm took in s:" << (double)(li2.QuadPart-li.QuadPart)/res.QuadPart <<"\n" <<std::endl;
#else
    std::cout << "msm took in us:" <<  mtime(tv1,tv2) <<"\n" <<std::endl;
#endif
    return 0;
}

