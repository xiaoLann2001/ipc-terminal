#include "helper_ptz.h"

#include "pantilt.h"

/**
 * @brief 创建一个新的 PTZConfiguration 对象，用于描述 PTZ 的配置
*/
struct tt__PTZConfiguration *helper_create_default_ptz_configuration(struct soap *soap) {
    // 创建一个新的 PTZConfiguration 对象
    struct tt__PTZConfiguration *ptz_configuration = soap_new_tt__PTZConfiguration(soap, -1);
    if (!ptz_configuration) return NULL;

    ptz_configuration->Name = soap_strdup(soap, "PTZConfig_1");
    ptz_configuration->token = soap_strdup(soap, "PTZConfigToken_1");
    ptz_configuration->UseCount = 1;
    ptz_configuration->NodeToken = soap_strdup(soap, "PTZNodeToken_1");

    // PanTiltLimits
    ptz_configuration->PanTiltLimits = soap_new_tt__PanTiltLimits(soap, -1);
    ptz_configuration->PanTiltLimits->Range = helper_create_space_2d_description(soap);

    // ZoomLimits
    ptz_configuration->ZoomLimits = soap_new_tt__ZoomLimits(soap, -1);
    ptz_configuration->ZoomLimits->Range = helper_create_space_1d_description(soap);

    // Optional fields
    ptz_configuration->DefaultPTZSpeed = NULL;
    ptz_configuration->DefaultPTZTimeout = NULL;
    ptz_configuration->Extension = NULL;

    return ptz_configuration;
}

/**
 * @brief 创建一个新的 Space2DDescription 对象，用于描述 PT 的空间范围
*/
struct tt__Space2DDescription *helper_create_space_2d_description(struct soap *soap) {
    struct tt__Space2DDescription *space_2d_description = soap_new_tt__Space2DDescription(soap, -1);
    if (!space_2d_description) return NULL;

    // 获取云台的范围
    float x_min, x_max, y_min, y_max;
    helper_do_ptz_get_pantilt_range(&x_min, &x_max, &y_min, &y_max);

    space_2d_description->URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
    space_2d_description->XRange = soap_new_tt__FloatRange(soap, -1);
    space_2d_description->XRange->Min = x_min;
    space_2d_description->XRange->Max = x_max;
    space_2d_description->YRange = soap_new_tt__FloatRange(soap, -1);
    space_2d_description->YRange->Min = y_min;
    space_2d_description->YRange->Max = y_max;

    return space_2d_description;
}

/**
 * @brief 创建一个新的 Space1DDescription 对象，用于描述 Z 的空间范围
*/
struct tt__Space1DDescription *helper_create_space_1d_description(struct soap *soap) {
    struct tt__Space1DDescription *space_1d_description = soap_new_tt__Space1DDescription(soap, -1);
    if (!space_1d_description) return NULL;

    space_1d_description->URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
    space_1d_description->XRange = soap_new_tt__FloatRange(soap, -1);
    space_1d_description->XRange->Min = 0.0;
    space_1d_description->XRange->Max = 1.0;
    
    return space_1d_description;
}

/**
 * @brief 创建一个新的 PTZNode 对象，用于描述 PTZ 节点
*/
struct tt__PTZNode *helper_create_ptz_node(struct soap *soap, int node_index) {
    struct tt__PTZNode *ptz_node = soap_new_tt__PTZNode(soap, -1);
    if (!ptz_node) return NULL;

    switch (node_index) {
        case 0:
            // <PTZNode>/<token>
            ptz_node->token = soap_strdup(soap, "PTZNodeToken_1");
            
            // <PTZNode>/<FixedHomePosition>
            ptz_node->FixedHomePosition = soap_new_xsd__boolean(soap, -1);
            *(ptz_node->FixedHomePosition) = xsd__boolean__false_;

            // <PTZNode>/<Name>
            ptz_node->Name = soap_strdup(soap, "PTZNODE_1");

            // <PTZNode>/<SupportedPTZSpaces>
            ptz_node->SupportedPTZSpaces = soap_new_tt__PTZSpaces(soap, -1);
            soap_default_tt__PTZSpaces(soap, ptz_node->SupportedPTZSpaces);
            // <PTZNode>/<SupportedPTZSpaces>/<AbsolutePanTiltPositionSpace>
            ptz_node->SupportedPTZSpaces->AbsolutePanTiltPositionSpace = helper_create_space_2d_description(soap);
            
            // <PTZNode>/<SupportedPTZSpaces>/<AbsoluteZoomPositionSpace>
            // 暂不支持缩放
            
            // <PTZNode>/<SupportedPTZSpaces>/<RelativePanTiltTranslationSpace>
            ptz_node->SupportedPTZSpaces->RelativePanTiltTranslationSpace = helper_create_space_2d_description(soap);

            // <PTZNode>/<SupportedPTZSpaces>/<RelativeZoomTranslationSpace>
            // 暂不支持缩放

            // <PTZNode>/<SupportedPTZSpaces>/<ContinuousPanTiltVelocitySpace>
            // 暂不支持连续移动

            // <PTZNode>/<SupportedPTZSpaces>/<ContinuousZoomVelocitySpace>
            // 暂不支持缩放

            // <PTZNode>/<SupportedPTZSpaces>/<PanTiltSpeedSpace>
            // 暂不支持速度

            // <PTZNode>/<SupportedPTZSpaces>/<ZoomSpeedSpace>
            // 暂不支持缩放

            // <PTZNode>/<Extension>
            // 暂不支持扩展
            break;
        default:
            break;
    }
    
    return ptz_node;
}




void helper_do_ptz_get_pantilt_range(float *x_min, float *x_max, float *y_min, float *y_max) {
    pantilt_get_range(&g_pantilt, x_min, x_max, y_min, y_max);
}

void helper_do_ptz_get_status(float *x, float *y) {
    pantilt_get_status(&g_pantilt, x, y);
}






void helper_do_ptz_relative_move(float delta_x, float delta_y) {
    pantilt_relative_move(&g_pantilt, delta_x, delta_y);
}

void helper_do_ptz_absolute_move(float x, float y) {
    pantilt_absolute_move(&g_pantilt, x, y);
}