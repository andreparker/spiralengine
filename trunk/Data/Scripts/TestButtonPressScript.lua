-- function test code
function OnButtonPress( window )

	local logger = Spiral.Log
	local gui    = Spiral.GUI

	local button = gui.Util.window_to_button(window)

	if button ~= nil then

		logger.Log_I( "Script1: executed... \n" )

		button:SetButtonPressScript("OnButtonPress2")
	end
end

function OnButtonPress2( window )

	local logger = Spiral.Log
	local gui    = Spiral.GUI

	local button = gui.Util.window_to_button( window )

	if button ~= nil then

		logger.Log_I( "Script2: executed...\n" )

		button:SetButtonPressScript("OnButtonPress")
	end
end
