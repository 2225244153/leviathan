--
-- DESCRIPTION
--
-- @AUTHOR **
-- @DATE ${date} ${time}
--

require "UnLua"

local BTS_Monster_PHZ_BattleService = Class()

function BTS_Monster_PHZ_BattleService:ReceiveActivationAI(OwnerController, ControlledPawn)
    print('hsp----------BTS_Monster_PHZ_BattleService:ReceiveActivationAI')
    UE4.UBTFunctionLibrary.SetBlackboardValueAsObject(self, self.TargetKeyName, self.Target)
end

function BTS_Monster_PHZ_BattleService:ReceiveDeactivationAI(OwnerController, ControlledPawn)
    print('hsp----------BTS_Monster_PHZ_BattleService:ReceiveDeactivationAI')
    UE4.UBTFunctionLibrary.SetBlackboardValueAsObject(self, self.TargetKeyName, nil)
end

function BTS_Monster_PHZ_BattleService:ReceiveSearchStartAI(OwnerController, ControlledPawn)
    self.Monster = ControlledPawn
    self.Target = ControlledPawn:GetBattleTarget()

    if self.Target == nil then
        return
    end
    --进入战斗状态后根据权重获取可用技能
    self:GetUsableSkillID()
    self:CheckBattleState()
end

function BTS_Monster_PHZ_BattleService:ReceiveTickAI(OwnerController, ControlledPawn, DeltaSeconds)

end

function BTS_Monster_PHZ_BattleService:GetUsableSkillID()
    self.SkillID = 0
    local usableSkillIDs = self.Monster.AbilitySystemComponent:GetUsableAbilities()
    local usableSkillIDsNum = usableSkillIDs:Num()
    if usableSkillIDsNum ~= 0 then
        local skillsWeight = UE4.TArray(0)
        for i = 1, usableSkillIDsNum do
            local tempSkillId = usableSkillIDs:Get(i)
            local skillData = UE4.USkillUtils.GetSkillDataBySkillID(tempSkillId)
            skillsWeight:Add(skillData.Weight)
        end
    
        local index = UE4.UGHCommonUtils.RandomIntegerByWeight(skillsWeight)
        self.SkillID = usableSkillIDs:Get(index + 1)
    end
    UE4.UBTFunctionLibrary.SetBlackboardValueAsInt(self, self.SkillIdKeyName, self.SkillID)
end

function BTS_Monster_PHZ_BattleService:CheckBattleState()
    --1.没有可用技能，执行战斗表演
    if self.SkillID == 0 then
        self:SetBattleState('AI.State.Battle.Perform')
        return
    end
    local skillData = UE4.USkillUtils.GetSkillDataBySkillID(self.SkillID)
    --2.判断距离是否足够释放技能，不够则执行追击
    local distance = UE4.UGHCommonUtils.CalcDistance(self.Monster, self.Target)
    if distance > skillData.Distance then
        self:SetBattleState('AI.State.Battle.Pursue')
        return
    end
    --3.距离足够判断角度是否满足释放技能，不够则执行旋转
    local angle = UE4.UGHCommonUtils.Calc2DAngleByForward(self.Monster, self.Target)
    if angle > skillData.Rotation then
        self:SetBattleState('AI.State.Battle.Turn')
        return
    end
    --4.条件都满足释放技能
    self:SetBattleState('AI.State.Battle.Attack')
end

function BTS_Monster_PHZ_BattleService:SetBattleState(stateName)
    self.Monster.StateComponent:SetStateByName(stateName)
end

return BTS_Monster_PHZ_BattleService