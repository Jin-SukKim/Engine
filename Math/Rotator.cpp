#include "pch.h"
#include "Rotator.h"

namespace JE {

const Rotator Rotator::Identity = { 0.f, 0.f, 0.f };

void Rotator::Clamp()
{
    Yaw = GetAxisClampValue(Yaw);
    Roll = GetAxisClampValue(Roll);
    Pitch = GetAxisClampValue(Pitch);
}

float Rotator::GetAxisClampValue(float value)
{
    // fmod�� ù��° ������ ��ȣ�� ���� ��ȣ�� return
    float angle = std::fmod(value, 360.f);
    // ������ ������ ���� ����� ��ȯ
    if (angle < 0.f)
        angle += 360.f;
    return angle;
}

void Rotator::GetLocalAxes(Vector3& outRight, Vector3& outUp, Vector3& outForward)
{
    // c = cos, s = sin
    float cy = 0.f, sy = 0.f, cr = 0.f, sr = 0.f, cp = 0.f, sp = 0.f;

    // �� ȸ������ sin, cos ��
    MathUtils::GetSinCos(sy, cy, Yaw);
    MathUtils::GetSinCos(sr, cr, Roll);
    MathUtils::GetSinCos(sp, cp, Pitch);

    // ��ķ� ���
    {
        //// Z���� �������� X��, Y���� ȸ�� (Roll ȸ��)
        //Matrix3x3 rollMat = { Vector3(cr, sr, 0.f), Vector3(-sr, cr, 0.f), Vector3::UnitZ };
        //// X���� �������� Y��, Z���� ȸ�� (Pitch ȸ��)
        //Matrix3x3 pitchMat = { Vector3::UnitX, Vector3(0.f, cp, sp), Vector3(0.f, -sp, cp) };
        //// Y���� �������� X��, Z���� ȸ�� (�޼� ��ǥ�迡���� Y�� ȸ���� �ݴ�� ��) (Yaw ȸ��)
        //Matrix3x3 yawMat = { Vector3(cy, 0.f, -sy), Vector3::UnitY, Vector3(sy, 0.f, cy) };

        //// Roll -> Pitch -> Yaw ȸ��
        //Matrix3x3 rotation = rollMat * pitchMat * yawMat;
        //outRight = rotation[0];
        //outUp = rotation[1];
        //outForward = rotation[2];
    }

    // X�� (Pitch)
    outRight = Vector3(cr * cy + sr * sp * sy, sr * cp, -cr * sy + sr * sp * cy);
    // Y�� (Yaw)
    outUp = Vector3(-sr * cy + cr * sp * sy, cr * cp, sr * sy + cr * sp * cy);
    // Z�� (Roll)
    outForward = Vector3(cp * sy, -sp, cp * cy);
}

std::wstring Rotator::ToString() const
{
    return std::format(L"Yaw : {:.3f}, Roll : {:.3f}, Pitch : {:.3f}", Yaw, Roll, Pitch);
}
}