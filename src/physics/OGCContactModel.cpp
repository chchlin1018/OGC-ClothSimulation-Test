#include "physics/OGCContactModel.h"
#include "physics/ClothSimulation.h"
#include <QDebug>
#include <algorithm>

namespace Physics {

OGCContactModel::OGCContactModel(float contactRadius)
    : m_contactRadius(contactRadius)
    , m_stiffness(1000.0f)
    , m_damping(50.0f)
{
    qDebug() << "OGC接觸模型初始化，接觸半徑:" << m_contactRadius;
}

void OGCContactModel::processContacts(const std::vector<ContactInfo>& contacts, float deltaTime) {
    if (contacts.empty()) return;
    
    // 處理每個接觸
    for (const auto& contact : contacts) {
        applyOGCForce(contact, deltaTime);
    }
}

void OGCContactModel::applyOGCForce(const ContactInfo& contact, float deltaTime) {
    if (!contact.particle || contact.particle->pinned) return;
    
    // 計算偏移幾何
    QVector3D offsetPosition = calculateOffsetGeometry(contact);
    
    // 計算接觸力
    QVector3D contactForce = calculateContactForce(contact);
    
    // 計算阻尼力
    QVector3D dampingForce = calculateDampingForce(contact);
    
    // 總力
    QVector3D totalForce = contactForce + dampingForce;
    
    // 應用力到粒子
    contact.particle->addForce(totalForce);
    
    // OGC特有的位置修正
    if (contact.penetrationDepth > 0) {
        QVector3D correction = contact.contactNormal * (contact.penetrationDepth * 0.8f);
        contact.particle->position += correction;
    }
}

QVector3D OGCContactModel::calculateOffsetGeometry(const ContactInfo& contact) {
    // 在接觸法線方向上偏移接觸半徑的距離
    return contact.contactPoint + contact.contactNormal * m_contactRadius;
}

QVector3D OGCContactModel::calculateContactForce(const ContactInfo& contact) {
    // 基於穿透深度的彈性力
    float penetration = std::max(0.0f, contact.penetrationDepth);
    return contact.contactNormal * (m_stiffness * penetration);
}

QVector3D OGCContactModel::calculateDampingForce(const ContactInfo& contact) {
    // 計算法線方向的速度分量
    float normalVelocity = QVector3D::dotProduct(contact.particle->velocity, contact.contactNormal);
    
    // 只在粒子向接觸面移動時應用阻尼
    if (normalVelocity < 0) {
        return contact.contactNormal * (m_damping * normalVelocity);
    }
    
    return QVector3D(0, 0, 0);
}

} // namespace Physics
