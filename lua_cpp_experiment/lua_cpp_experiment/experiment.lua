function execute()
	this.entity:AddComponentX()
	this.entity:AddComponentY()

	local component_x = this.entity:GetComponentX()
	local component_y = this.entity:GetComponentY()

	component_x:DoStuff()

	component_y:SetYValue(4.5)

	print("Get value gives "..component_y:GetYValue())

	print("SetStuff is called on type ComponentY:")
	component_y:SetStuff(0.5, 0.2, 0.3)

	print("GetComponents is called on this.entity")
	local all_components = this.entity:GetComponents()

	for i = 1, #all_components do
		local type = all_components[i]:GetType()

		if type == this.ComponentType.NONE then
			print("We have a NONE")
		elseif type == this.ComponentType.X then
			print("We have an X")
		elseif type == this.ComponentType.Y then
			print("We have a Y")
		else 
			print("This should not happen as we only have 3 enums")
		end
	end

end