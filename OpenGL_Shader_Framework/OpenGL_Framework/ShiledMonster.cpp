#include "pch.h"
#include "ShiledMonster.h"
#include"NormalMonster.h"
CShiledMonster::CShiledMonster(GLuint* _shader_program, glm::vec3 pos)
    :CObj(_shader_program)
{
    Initialize();
    object.model_transform.Translate = pos;
    m_fSpeed = 0.2f;
    Look = { 0.f,1.f,0.f };
    object.model_transform.Scale *= 5.f;
}

CShiledMonster::~CShiledMonster()
{
    Release();
}

void CShiledMonster::Initialize()
{
    loadObj<const char*, OBJECT, glm::vec3>("shield.obj", object, glm::vec3(0.5f, 0.f, 0.f));


}

int CShiledMonster::Update()
{
    if (m_bDead) {
        if (m_CollisionCount < 2) {
            m_CollisionCount++;
            m_bDead = false;
        }
        else {
            CSoundMgr::GetInstance()->PlaySound(L"die.mp3", CSoundMgr::EFFECT);
            Add_Effect(20);
            return DEAD_OBJ;

        }

    }
    object.model_transform.Translate.x += m_fSpeed;

    if (object.model_transform.Translate.x <= -20.f || object.model_transform.Translate.x >= 20.f)
        m_fSpeed *= -1;


    return 0;

}

void CShiledMonster::Late_Update()
{
}

void CShiledMonster::Draw()
{
    glLineWidth(6.f);

    unsigned int modelLocation = glGetUniformLocation(*shader_program, "model");
    unsigned int viewLocation = glGetUniformLocation(*shader_program, "view");
    unsigned int projLocation = glGetUniformLocation(*shader_program, "projection");
    // ============================= ???? ============================================


    // ============================= ?? ??ȯ ============================================
    // ī?޶? ??ǥ ????
    glm::vec3 cameraPos = { 0.f,1.f,5.f };
    glm::vec3 cameraDirection = { 0.f,0.f,-1.f };
    glm::vec3 cameraUp = { 0.f,1.f,0.f };




    glm::mat4 view = glm::mat4(1.f);
    glm::mat4 view_Rotate = glm::mat4(1.f);
    view_Rotate = glm::rotate(view_Rotate, glm::radians(0.f), glm::vec3(0.0, 1.0, 0.0));

    CAMERA_DESC camera = CObjectMgr::Get_Instance()->Get_Camera();
    view = view_Rotate * glm::lookAt(camera.cameraPos, camera.cameraDirection, camera.cameraUp);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
    // ============================= ?? ??ȯ ============================================

    // ============================= ???? ??ȯ ============================================
    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(glm::radians(FOVY), (float)WINCX / (float)WINCY, 1.f, FAR_Z);
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
    // ============================= ???? ??ȯ ============================================

    glm::mat4 Scale = glm::mat4(1.f);

    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 RotateZ = glm::mat4(1.f);

    glm::mat4 Translate = glm::mat4(1.f);

    glm::mat4 RevolutionX = glm::mat4(1.f);
    glm::mat4 RevolutionY = glm::mat4(1.f);
    glm::mat4 RevolutionZ = glm::mat4(1.f);

    glm::mat4 Parent = glm::mat4(1.f);

    glm::mat4 Result = glm::mat4(1.f);

    Scale = glm::scale(Scale, object.model_transform.Scale);

    RotateX = glm::rotate(RotateX, glm::radians(object.model_transform.Rotate.x = { 0.f }), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(object.model_transform.Rotate.y = { 0.f }), glm::vec3(0.0, 1.0, 0.0));
    RotateZ = glm::rotate(RotateZ, glm::radians(object.model_transform.Rotate.z = { 0.f }), glm::vec3(0.0, 0.0, 1.0));

    Translate = glm::translate(Translate, object.model_transform.Translate);

    RevolutionX = glm::rotate(RevolutionX, glm::radians(object.model_transform.Revolution.x), glm::vec3(1.0, 0.0, 0.0));
    RevolutionY = glm::rotate(RevolutionY, glm::radians(object.model_transform.Revolution.y), glm::vec3(0.0, 1.0, 0.0));
    RevolutionZ = glm::rotate(RevolutionZ, glm::radians(object.model_transform.Revolution.z), glm::vec3(0.0, 0.0, 1.0));
    if (object.model_transform.Parent)
        Parent = glm::translate(Parent, object.model_transform.Parent->Translate);

    // ũ ?? ?? ?? ?? ?Ųٷ?
    Result = Parent * RevolutionX * RevolutionY * RevolutionZ * Translate * RotateX * RotateY * RotateZ * Scale;

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Result));

    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_LOOP, 0, object.vertices.size());

    glLineWidth(1.f);

}

void CShiledMonster::Release()
{
}

void CShiledMonster::Set_ModelTransform()
{
}

