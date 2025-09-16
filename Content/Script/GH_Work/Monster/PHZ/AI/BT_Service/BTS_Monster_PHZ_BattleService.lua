--
-- DESCRIPTION
--
-- @AUTHOR **
-- @DATE ${date} ${time}
--

require "UnLua"

local BTS_Monster_PHZ_BattleService = Class()

function BTS_Monster_PHZ_BattleService:ReceiveActivationAI(OwnerController, ControlledPawn)
    UE.UBTFunctionLibrary.SetBlackboardValueAsObject(self, self.TargetKeyName, self.Target)
end

function BTS_Monster_PHZ_BattleService:ReceiveDeactivationAI(OwnerController, ControlledPawn)
    UE.UBTFunctionLibrary.SetBlackboardValueAsObject(self, self.TargetKeyName, nil)
    UE.UBTFunctionLibrary.SetBlackboardValueAsInt(self, self.SkillIdKeyName, 0)
    self.SkillID = 0
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
    local bBattleTag = UE.UBlueprintGameplayTagLibrary.MatchesTag(self.Monster.StateComponent:GetState(), self.BattleTag, false)
    if not bBattleTag then
        return
    end
    if self.Target == nil then
        return
    end
    self:GetUsableSkillID()
    self:CheckBattleState()
end

function BTS_Monster_PHZ_BattleService:GetUsableSkillID()
    if self.SkillID  ~= nil and self.SkillID ~= 0 then
        return
    end
    
    self.SkillID = 0
    local usableSkillIDs = self.Monster.AbilitySystemComponent:GetUsableAbilities()
    local usableSkillIDsNum = usableSkillIDs:Num()
    if usableSkillIDsNum ~= 0 then
        local skillsWeight = UE.TArray(0)
        for i = 1, usableSkillIDsNum do
            local tempSkillId = usableSkillIDs:Get(i)
            local skillData = UE.USkillUtils.GetSkillDataBySkillID(tempSkillId)
            skillsWeight:Add(skillData.Weight)
        end
    
        local index = UE.UGHCommonUtils.RandomIntegerByWeight(skillsWeight)
        self.SkillID = usableSkillIDs:Get(index + 1)
    end
    UE.UBTFunctionLibrary.SetBlackboardValueAsInt(self, self.SkillIdKeyName, self.SkillID)
end

function BTS_Monster_PHZ_BattleService:CheckBattleState()
    --1.没有可用技能，执行战斗表演
    if self.SkillID == 0 then
        self.Monster.StateComponent:SetState(self.BattlePerformTag)
        return
    end
    local skillData = UE.USkillUtils.GetSkillDataBySkillID(self.SkillID)
    --2.判断距离是否足够释放技能，不够则执行追击
    local distance = UE.UGHCommonUtils.CalcDistance(self.Monster, self.Target)
    if distance > skillData.Distance then
        self.Monster.StateComponent:SetState(self.BattlePursueTag)
        return
    end
    --3.距离足够判断角度是否满足释放技能，不够则执行旋转
    local angle = UE.UGHCommonUtils.Calc2DAngleByForward(self.Monster, self.Target)
    if angle > (skillData.Rotation / 2) then
        self.Monster.StateComponent:SetState(self.BattleTurnTag)
        return
    end
    --4.条件都满足释放技能
    self.Monster.StateComponent:SetState(self.BattleAttackTag)
end

return BTS_Monster_PHZ_BattleService