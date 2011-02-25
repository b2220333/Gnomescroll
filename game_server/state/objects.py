

crops_dat = {
	generic_crop: 
	{
		template_params : ['id','position', 'version', 'world_id'],
		template : 
		{
			name : "generic_crop",
			type : ["crop"],
			parent : "generic_crop",
			#version : 0
		},

		plantable : 1,
		planting_experience : 20 ,
		planting_difficulty : 1,
		grows_from : "generic_seeds",
		
		harvestable : 0,
		matures : 1,
		mature_time : 900,
		matures_into : "generic_mature_crop"
	},

	generic_mature_crop:
	{
		template_params : ['id','position', 'version', 'world_id'],
		template : 
		{
			name : "generic_mature_crop",
			type : ["crop"],
			parent : "generic_mature_crop",
			#version : 0
		},
		plantable : 0,
		harvestable : 1,
		harvest_produces : [(1,3, 'generic_food')], # 1d3 generic food
		harvest_experience : 10,
		matures : 0, #disable maturing for now
		mature_time : 5000,
		matures_into : "generic_dead_crop"
	},

	generic_dead_crop:
	{
		template_params : ['id','position', 'version', 'world_id'],
		template : 
		{
			name : "generic_dead_crop",
			type : ["crop"],
			parent : "generic_dead_crop",
			#version : 0
		},
		plantable : 0,
		harvestable : 1,
		harvest_produces : [], # 1d3 generic food
		harvest_experience : 5,
		matures : 0
	}
	
}

def create_crop(id, x,y,z,template = None, world_id = 0):
	if template == None:
		template = 'generic_crop'
	a = crops_dat[template]
	
	a = {}
	a['id'] = id
	#position is (type, x, y, z)
	a['position'] = [0 , x, y, z] #type zero is ground
	a['world_id'] = world_id
	a['version'] = 0
	return a

#creates an oject from a template
def create_object(id, x, y, z, template = None, player_id = 0, world_id = 0):
	#if prototype == None:
	#	pass
	a = {}
	a['id'] = id
	a['type'] = ['item']
	a['name'] = 'generic item'
	#position is (type, x, y, z)
	a['position'] = [0 , x, y, z] #type zero is ground
	a['player_id'] = player_id
	a['world_id'] = world_id
	a['version'] = 0
	return a
	
class Objects:

	def __init__(self):
		self.globals = None
		self.delta = None
		#
		self.object_list = {}

	def init(self):
		world_id = self.globals.world_id
		#grab state from persistant store
		pass

	def create(self, position, object_type = None, template = None, player_id=0):
		(position_type, x, y, z) = position
		
		id = self.globals.get_unique_id()

		if object_type == None:
			a = create_object(id, x, y, z, player_id=player_id, world_id = self.globals.world_id)
		if object_type == 'item':
			pass
		if object_type == 'crop':
			pass
		if object_type == 'plant':
			pass
		self.object_list[id] = a
		self.delta.object_create(id, position, a['type'])
		
	def delete(self, id):
		pass
