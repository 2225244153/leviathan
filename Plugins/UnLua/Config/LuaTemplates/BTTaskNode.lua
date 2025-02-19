--
-- DESCRIPTION
--
-- @AUTHOR **
-- @DATE ${date} ${time}
--

require "UnLua"

local TemplateName = Class()

--需要主动调用 self:FinishExecute(bSuccess)或者self:FinishAbort()

-- function TemplateName:ReceiveExecuteAI(OwnerController, ControlledPawn)
-- end

-- function TemplateName:ReceiveAbortAI(OwnerController, ControlledPawn)
-- end

-- function TemplateName:ReceiveTickAI(OwnerController, ControlledPawn, DeltaSeconds)
-- end

return TemplateName