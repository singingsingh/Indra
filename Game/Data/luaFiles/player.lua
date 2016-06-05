player =
{
	UID = 1,
	name = "GoodGuy",
	position = { -180.0, 0.0 },
	rotDegree = 0.0,
	
	physicsInfo = 
	{
		mass = 20.0,
		drag = 0.99
	},
	
	renderInfo =
	{
		sprite = "data\\sprites\\kid.dds"
	},
	
	controller = "playerController",
	
	collider =
	{
		tag = "player",
		numCollisionTag = 1,
		collidesWith = 
		{
			"npc"
		}
	}
}