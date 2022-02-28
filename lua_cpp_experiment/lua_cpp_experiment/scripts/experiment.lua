experiment = {
  name = "fuck",
  number = 69
}

function experiment:execute()
  -- require('mobdebug').start()
  -- Add x and y components to the entity we have
  -- by calling AddComponent<ComponentX> and 
  -- AddComponent<ComponentY> of Entitiy:
  hachiko.entity:AddComponentX()
  hachiko.entity:AddComponentY()

  -- Get the references of the components we have:
  local component_x = hachiko.entity:GetComponentX()
  local component_y = hachiko.entity:GetComponentY()

  hachiko.entity.ID = 5

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
  local all_components = hachiko.entity:GetComponents()

  -- Loop through the components of entity:
  -- # is the length operator, 
  -- arrays start at 1 for whatever the fucking reason.
  for i = 1, #all_components do
    -- Call GetType of Component, this is returned as integer:
    local type = all_components[i]:GetType()
    
    -- Since we also registered ComponentType enum to this instance
    -- of lua, we can say this.ComponentType.X, .Y, .NONE:
    if type == hachiko.ComponentType.NONE then
      print("We have a NONE")
    elseif type == hachiko.ComponentType.X then
      print("We have an X")
    elseif type == hachiko.ComponentType.Y then
      print("We have a Y")
    else 
      print("This should not happen as we only have 3 enums")
    end
  end
end

function experiment:call_from_another_script(an_integer, a_string)
  print("The previous values were: "..experiment.name.." and "..experiment.number)

  self.number = an_integer
  self.name = a_string

  print("This was called from another script, well done, my dude. The values are "..experiment.name.." and "..experiment.number)
end
