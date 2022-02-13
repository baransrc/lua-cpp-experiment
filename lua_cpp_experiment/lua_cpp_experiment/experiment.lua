function execute()
	-- Add x and y components to the entity we have
	-- by calling AddComponent<ComponentX> and 
	-- AddComponent<ComponentY> of Entitiy:
	this.entity:AddComponentX()
	this.entity:AddComponentY()

	-- Get the references of the components we have:
	local component_x = this.entity:GetComponentX()
	local component_y = this.entity:GetComponentY()

	-- Call DoStuff method of component_x:
	component_x:DoStuff()
	-- Call SetYValue method of component_y:
	component_y:SetYValue(4.5)

	-- Call GetYValue method of 
	print("Get value gives "..component_y:GetYValue())

	-- Call SetStuff method that base Component has from
	-- component_y of type ComponentY:
	print("SetStuff is called on type ComponentY:")
	component_y:SetStuff(0.5, 0.2, 0.3)

	-- Call GetComponents method of Entity:
	-- This gets std::vector as array.
	print("GetComponents is called on this.entity")
	local all_components = this.entity:GetComponents()

	-- Loop through the components of entity:
	-- # is the length operator, 
	-- arrays start at 1 for whatever the fucking reason.
	for i = 1, #all_components do
		-- Call GetType of Component, this is returned as integer:
		local type = all_components[i]:GetType()
		
		-- Since we also registered ComponentType enum to this instance
		-- of lua, we can say this.ComponentType.X, .Y, .NONE:
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