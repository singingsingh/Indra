#include <Engine\Engine\Collision.h>
#include <Engine\Core\Math\Matrix4.h>
#include <Engine\Engine\Physicsinfo.h>
#include <Engine\Engine\GameObject.h>
#include <Engine\Util\MathUtils.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Engine\HitResult.h>

#include <vector>
#include <math.h>
#include <limits>

namespace Engine
{
	namespace Collision
	{
		void CheckCollisionOn(std::vector<Math::Vector2f>& meBB, std::vector<Math::Vector2f>& otherBB, const Math::Vector2f i_meVelocity, const Math::Vector2f i_otherVelocity, bool& collision)
		{
			// calculate edge normal
			std::vector<Math::Vector2f>::iterator itr = meBB.begin();
			Math::Vector2f startEdge, endEdge, edge, normalEdge;
			float cos90 = cosf(MathUtils::ToRadians(90));
			float sin90 = sinf(MathUtils::ToRadians(90));

			for (; itr != meBB.end() && collision; itr++)
			{
				startEdge = (*itr);

				if (itr == meBB.end() - 1)
				{
					endEdge = *(meBB.begin());
				}
				else
				{
					endEdge = *(itr + 1);
				}

				edge = endEdge - startEdge;

				// rotate the edge
				normalEdge.x = edge.x*cos90 - edge.y*sin90;
				normalEdge.y = edge.x*sin90 + edge.y*cos90;
				normalEdge.normalize();

				// project and calculate the bounds
				float meMin = std::numeric_limits<float>::max();
				float meMax = -std::numeric_limits<float>::max();
				float otherMin = std::numeric_limits<float>::max();
				float otherMax = -std::numeric_limits<float>::max();
				float dotValue = 0.0f;
				float meExtends = 0.0f;
				float otherExtends = 0.0f;
				float meProjPos = 0.0f;
				float otherProjPos = 0.0f;

				for (std::vector<Math::Vector2f>::iterator meItr = meBB.begin(); meItr != meBB.end(); meItr++)
				{
					dotValue = dot((*meItr), normalEdge);

					if (dotValue < meMin)
					{
						meMin = dotValue;
					}
					if (dotValue > meMax)
					{
						meMax = dotValue;
					}
				}
				meExtends = fabs(meMax - meMin);
				meProjPos = (meMin + meMax)*0.5f;

				for (std::vector<Math::Vector2f>::iterator otherItr = otherBB.begin(); otherItr != otherBB.end(); otherItr++)
				{
					dotValue = dot((*otherItr), normalEdge);

					if (dotValue < otherMin)
					{
						otherMin = dotValue;
					}
					if (dotValue > otherMax)
					{
						otherMax = dotValue;
					}
				}
				otherExtends = fabs(otherMax - otherMin);
				otherProjPos = (otherMin + otherMax)*0.5f;

				// making me fat by other
				meMax += (otherExtends / 2.0f);
				meMin -= (otherExtends / 2.0f);
				meExtends = fabs(meMax - meMin);

				// get projected relative velocity
				Math::Vector2f relativeVel;
				relativeVel.x = i_meVelocity.x - i_otherVelocity.x;
				relativeVel.y = i_meVelocity.y - i_otherVelocity.y;
				float projRelativeVel = dot(relativeVel, normalEdge);

				// making me fat by projected velocity
				if (projRelativeVel > 0.0f)
				{
					meMax += projRelativeVel;
				}
				else
				{
					meMin += projRelativeVel;
				}
				meExtends = fabs(meMax - meMin);

				if (otherProjPos < meMax && otherProjPos > meMin)
				{
					collision = true;
				}
				else
				{
					collision = false;
					break;
				}
			}
		}

		void CheckCollision(IPhysicsTickListener* i_me, IPhysicsTickListener* i_other, float deltaTime)
		{
			HitResult o_retVal;

			bool collision = true;
			Math::Vector3 mePos;
			mePos.x( i_me->_physicsInfo->_gameObject->pos.x );
			mePos.y(i_me->_physicsInfo->_gameObject->pos.y);
			mePos.z(0.0f);
			Math::Matrix4 meModelTrans = Math::Matrix4::CreateTranslationCV(mePos);
			Math::Matrix4 meModelRot = Math::Matrix4::CreateZRotationCV( MathUtils::ToRadians(i_me->_physicsInfo->_gameObject->rotDegree) );
			Math::Matrix4 meModel = meModelTrans*meModelRot;

			Math::Vector3 otherPos;
			otherPos.x( i_other->_physicsInfo->_gameObject->pos.x );
			otherPos.y(i_other->_physicsInfo->_gameObject->pos.y);
			otherPos.z(0.0f);
			Math::Matrix4 otherModelTrans = Math::Matrix4::CreateTranslationCV(otherPos);
			Math::Matrix4 otherModelRot = Math::Matrix4::CreateZRotationCV( MathUtils::ToRadians( i_other->_physicsInfo->_gameObject->rotDegree ) );
			Math::Matrix4 otherModel = otherModelTrans*otherModelRot;

			std::vector<Math::Vector2f> meBB = i_me->_physicsInfo->boundingBox;
			std::vector<Math::Vector2f> otherBB = i_other->_physicsInfo->boundingBox;

			// convert the points to world space
			std::vector<Math::Vector2f>::iterator itr = meBB.begin();
			Math::Vector4 tempVec4;

			for (; itr != meBB.end(); itr++)
			{
				tempVec4.x((*itr).x);
				tempVec4.y((*itr).y);
				tempVec4.z(0.0f);
				tempVec4.w(1.0f);
				tempVec4 = meModel * tempVec4;
				(*itr).x = tempVec4.x();
				(*itr).y = tempVec4.y();
			}

			itr = otherBB.begin();
			for (; itr != otherBB.end(); itr++)
			{
				tempVec4.x((*itr).x);
				tempVec4.y((*itr).y);
				tempVec4.z(0.0f);
				tempVec4.w(1.0f);
				tempVec4 = otherModel * tempVec4;
				(*itr).x = tempVec4.x();
				(*itr).y = tempVec4.y();
			}

			// calcalte projections on me edges

			CheckCollisionOn(meBB, otherBB, i_me->_physicsInfo->velocity, i_other->_physicsInfo->velocity, collision);
			if (collision)
			{
				CheckCollisionOn(otherBB, meBB, i_other->_physicsInfo->velocity, i_me->_physicsInfo->velocity, collision);
			}

			if (collision)
			{
				//DEBUG_PRINT("Collision\n");
				i_me->_physicsInfo->hitResult->collision = true;
				i_other->_physicsInfo->hitResult->collision = true;

				i_me->_physicsInfo->hitResult->other = i_other->_physicsInfo;
				i_other->_physicsInfo->hitResult->other = i_me->_physicsInfo;
			}

			meBB.clear();
			otherBB.clear();
		}
	}
}