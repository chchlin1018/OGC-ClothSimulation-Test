#pragma once

#include <vector>
#include <QVector3D>

namespace Physics {

class ClothParticle;

/**
 * @brief OGC (Offset Geometry Contact) 接觸模型
 * 
 * 這個類別實現了偏移幾何接觸模型，用於處理布料與剛體之間的接觸。
 * OGC模型通過在接觸點周圍創建偏移幾何來提供更穩定和真實的接觸響應。
 */
class OGCContactModel {
public:
    /**
     * @brief 接觸資訊結構
     */
    struct ContactInfo {
        ClothParticle* particle;        ///< 參與接觸的粒子
        QVector3D contactPoint;         ///< 接觸點位置
        QVector3D contactNormal;        ///< 接觸法線
        float penetrationDepth;         ///< 穿透深度
        float contactRadius;            ///< 接觸半徑
    };
    
    /**
     * @brief 構造函數
     * @param contactRadius 接觸半徑，用於定義偏移幾何的大小
     */
    explicit OGCContactModel(float contactRadius = 0.1f);
    
    /**
     * @brief 析構函數
     */
    ~OGCContactModel() = default;
    
    /**
     * @brief 處理一組接觸
     * @param contacts 接觸資訊列表
     * @param deltaTime 時間步長
     */
    void processContacts(const std::vector<ContactInfo>& contacts, float deltaTime);
    
    /**
     * @brief 設定接觸半徑
     * @param radius 新的接觸半徑
     */
    void setContactRadius(float radius) { m_contactRadius = radius; }
    
    /**
     * @brief 獲取接觸半徑
     * @return 當前接觸半徑
     */
    float getContactRadius() const { return m_contactRadius; }
    
    /**
     * @brief 設定剛度係數
     * @param stiffness 剛度係數
     */
    void setStiffness(float stiffness) { m_stiffness = stiffness; }
    
    /**
     * @brief 獲取剛度係數
     * @return 當前剛度係數
     */
    float getStiffness() const { return m_stiffness; }
    
    /**
     * @brief 設定阻尼係數
     * @param damping 阻尼係數
     */
    void setDamping(float damping) { m_damping = damping; }
    
    /**
     * @brief 獲取阻尼係數
     * @return 當前阻尼係數
     */
    float getDamping() const { return m_damping; }

private:
    float m_contactRadius;      ///< 接觸半徑
    float m_stiffness;          ///< 剛度係數
    float m_damping;            ///< 阻尼係數
    
    /**
     * @brief 對單個接觸應用OGC力
     * @param contact 接觸資訊
     * @param deltaTime 時間步長
     */
    void applyOGCForce(const ContactInfo& contact, float deltaTime);
    
    /**
     * @brief 計算偏移幾何
     * @param contact 接觸資訊
     * @return 偏移後的位置
     */
    QVector3D calculateOffsetGeometry(const ContactInfo& contact);
    
    /**
     * @brief 計算接觸力
     * @param contact 接觸資訊
     * @return 接觸力向量
     */
    QVector3D calculateContactForce(const ContactInfo& contact);
    
    /**
     * @brief 計算阻尼力
     * @param contact 接觸資訊
     * @return 阻尼力向量
     */
    QVector3D calculateDampingForce(const ContactInfo& contact);
};

} // namespace Physics
