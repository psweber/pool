# include "CollisionSet.hpp"
# include "Objects.hpp"

engine::CollisionSet::CollisionSet
(
	ShapeType shapeI,
	ShapeType shapeII,
	ObjectType objectType,
	CollisionType collisionType
)
:
shapeI_(shapeI),
shapeII_(shapeII),
objectType_(objectType)
{
	// Create collision model
	switch (collisionType)
	{
		case CollisionType::ELASTIC:
			model_ =
				std::shared_ptr<CollisionModel>(new ElasticCollision());
	}
}


void engine::CollisionSet::collide
(
	std::vector<std::shared_ptr<CollisionSet>> sets,
	std::shared_ptr<MovingObject> obj1,
	std::shared_ptr<Object> obj2,
	double deltaT
)
{
	ShapeType shape1 = obj1->shape()->type();
	ShapeType shape2 = obj2->shape()->type();
	ObjectType objType = obj2->type();
	
	// Run through collision sets and apply the first one matching
	for (auto s : sets)
	{
		if
		(
			(shape1 == s->shapeI())
		 && (shape2 == s->shapeII())
		 && (objType == s->objectType())
		)
		{
			s->model()->collide(obj1,obj2,deltaT);
			return;
		}
	}
}

bool engine::CollisionSet::clip
(
	std::vector<std::shared_ptr<CollisionSet>> sets,
	std::shared_ptr<MovingObject> obj1,
	std::shared_ptr<Object> obj2
)
{
	ShapeType shape1 = obj1->shape()->type();
	ShapeType shape2 = obj2->shape()->type();
	ObjectType objType = obj2->type();
	
	// Run through collision sets and return true if one matches
	for (auto s : sets)
	{
		if
		(
			(shape1 == s->shapeI())
		 && (shape2 == s->shapeII())
		 && (objType == s->objectType())
		)
		{
			return false;
		}
	}
	
	return true;
}
