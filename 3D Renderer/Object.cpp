#include "Object.h"
#include "Math.h"

Object::Object(class Game* game) {
	this->model = Model();
	this->location = Vector3::Zero;
	this->direction = Vector3::UnitZ;
	this->rotation = 0;
	this->game = game;
	return;
}
Object::Object() {
	Object(NULL);
	return;
}
Object::~Object() {
	return;
}

void Object::processInput(const Uint8* keyBoardState) {
	if (keyBoardState[SDL_SCANCODE_W]) { move(Vector3(0, 0, +50)); }
	if (keyBoardState[SDL_SCANCODE_A]) { move(Vector3(-50, 0, 0)); }
	if (keyBoardState[SDL_SCANCODE_S]) { move(Vector3(0, 0, -50)); }
	if (keyBoardState[SDL_SCANCODE_D]) { move(Vector3(+50, 0, 0)); }
}

void Object::update() {
	return;
}

void Object::moveTo(const Vector3 location) {
	this->location = location;
	this->model.moveTo(location);
}

void Object::move(const Vector3& movement) {
	Matrix4 transformMat = getBasisConversion();
	transformMat.Invert();
	Vector3 movementWithDirection = Vector3::Transform(movement, transformMat);

	location += movementWithDirection;
	model.move(movementWithDirection);
}

//void Object::rotateBySelf(const Vector3& rotateAxis, float radian) {
//	Matrix4 invertedBasisConversion = getBasisConversion(); invertedBasisConversion.Invert();
//	Vector3 tempAxis = Vector3::Normalize(Vector3::Transform(rotateAxis, invertedBasisConversion));
//	rotate(tempAxis, radian);
//}
void Object::rotate(const Vector3& rotateAxis, float radian) {
	direction = Vector3::Transform(direction, Quaternion(rotateAxis, radian));
	model.rotate(rotateAxis, radian);
}
void Object::rotateX(float radian) {
	rotate(Vector3::UnitX, radian);
}
void Object::rotateY(float radian) {
	rotate(Vector3::UnitY, radian);
}
void Object::rotateZ(float radian) {
	rotate(Vector3::UnitZ, radian);
}
void Object::lookAt(Vector3 targetLocation) {
	Vector3 targetDirection = targetLocation - location;

	Vector3 axis = Vector3::Normalize(Vector3::Cross(direction, targetDirection));
	float rAngle = Math::Acos(Vector3::Dot(direction, targetDirection) / (direction.Length() * targetDirection.Length()));
	if (0.01f <= Math::Abs(rAngle) && Math::Abs(rAngle) <= 10) // 근데 빌어먹을 부동소숫점에서 수가 미친듯이 커지는 경우가 어디있냐고
		rotate(axis, rAngle);

	//// yz : 상하, xz : 좌우
	//Vector3 directionYZ = direction * Vector3(0, 1, 1);
	//Vector3 targetDirYZ = targetDirection * Vector3(0, 1, 1);
	//Vector3 axisYZ = Vector3::Normalize(Vector3::Cross(directionYZ, targetDirYZ));
	//float angleYZ = Math::Acos(Vector3::Dot(directionYZ, targetDirYZ) / (directionYZ.Length() * targetDirYZ.Length()));

	//Vector3 directionXZ = direction * Vector3(1, 0, 1);
	//Vector3 targetDirXZ = targetDirection * Vector3(1, 0, 1);
	//Vector3 axisXZ = Vector3::Normalize(Vector3::Cross(directionYZ, targetDirYZ));
	//float angleXZ = Math::Acos(Vector3::Dot(directionYZ, targetDirYZ) / (directionYZ.Length() * targetDirYZ.Length()));


	//if (/*0.01f <= Math::Abs(angleYZ) && */Math::Abs(angleYZ) <= 10)
	//	rotate(axisYZ, angleYZ);
	//if (/*0.01f <= Math::Abs(angleXZ) && */Math::Abs(angleXZ) <= 10)
	//	rotate(axisXZ, angleXZ);
}

const Model& Object::getModel() {
	return this->model;
}
void Object::setModel(Model model) {
	this->model = model;
	this->model.moveTo(location);
}
void Object::scale(float scale) {
	model.scale(scale);
}

Matrix4 Object::getBasisConversion() const { //멤버를 const로 만들기가 가능하다면서 그렇게 되어버렸다. 이거 언제 가능한 문법이지?
	Vector3 zBasis = Vector3::Normalize(direction);
	Vector3 xBasis = Vector3::Cross(Vector3::UnitY, zBasis);
	//Vector3 xBasis = Vector3::Cross(zBasis, Vector3::UnitY);
	Vector3 yBasis = Vector3::Cross(zBasis, xBasis);
	//Vector3 yBasis = Vector3::Cross(xBasis, zBasis);
	float basisVectors[4][4] = {
		{ xBasis.x, yBasis.x, zBasis.x, 0 },
		{ xBasis.y, yBasis.y, zBasis.y, 0 },
		{ xBasis.z, yBasis.z, zBasis.z, 0 },
		{ 0, 0, 0, 1 }
	 };
	return Matrix4(basisVectors);
}
