npc =
{
	UID = 2,
	name = "BadGuy",
	position = { 180.0, 0.0 },
	rotDegree = 0.0,
	
	physicsInfo = 
	{
		mass = 5.0,
		drag = 0.99
	},
	
	renderInfo =
	{
		sprite = "data\\sprites\\ghost.dds"
	},
	
	controller = "monsterController",
	
	collider =
	{
		tag = "npc",
		numCollisionTag = 1,
		collidesWith = 
		{
			"player"
		}
	}
}