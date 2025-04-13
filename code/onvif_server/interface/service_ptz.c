#include "soapH.h"
#include "soapStub.h"

#include "global.h"

/** Web service operation '__ns1__GetConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetConfigurations(struct soap* soap, struct _ns1__GetConfigurations *ns1__GetConfigurations, struct _ns1__GetConfigurationsResponse *ns1__GetConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__GetPresets' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetPresets(struct soap* soap, struct _ns1__GetPresets *ns1__GetPresets, struct _ns1__GetPresetsResponse *ns1__GetPresetsResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__SetPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetPreset(struct soap* soap, struct _ns1__SetPreset *ns1__SetPreset, struct _ns1__SetPresetResponse *ns1__SetPresetResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__RemovePreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__RemovePreset(struct soap* soap, struct _ns1__RemovePreset *ns1__RemovePreset, struct _ns1__RemovePresetResponse *ns1__RemovePresetResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__GotoPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GotoPreset(struct soap* soap, struct _ns1__GotoPreset *ns1__GotoPreset, struct _ns1__GotoPresetResponse *ns1__GotoPresetResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__GetStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetStatus(struct soap* soap, struct _ns1__GetStatus *ns1__GetStatus, struct _ns1__GetStatusResponse *ns1__GetStatusResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__GetConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetConfiguration(struct soap* soap, struct _ns1__GetConfiguration *ns1__GetConfiguration, struct _ns1__GetConfigurationResponse *ns1__GetConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__GetNodes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetNodes(struct soap* soap, struct _ns1__GetNodes *ns1__GetNodes, struct _ns1__GetNodesResponse *ns1__GetNodesResponse) {
    LOG_DEBUG("__ns1__GetNodes");

    // Node 个数
    ns1__GetNodesResponse->__sizePTZNode = 1;

    // <PTZNodes>
    ns1__GetNodesResponse->PTZNode = soap_new_tt__PTZNode(soap, ns1__GetNodesResponse->__sizePTZNode);
    for (int i = 0; i < ns1__GetNodesResponse->__sizePTZNode; i++) {
        soap_default_tt__PTZNode(soap, &(ns1__GetNodesResponse->PTZNode[i]));
    }

    // PTZNode_1
    int i = 0;
    // <PTZNode>/<token>
    ns1__GetNodesResponse->PTZNode[i].token = soap_strdup(soap, "PTZNodeToken1");
    
    // <PTZNode>/<FixedHomePosition>
    ns1__GetNodesResponse->PTZNode[i].FixedHomePosition = soap_new_xsd__boolean(soap, -1);
    ns1__GetNodesResponse->PTZNode[i].FixedHomePosition = xsd__boolean__false_;

    // <PTZNode>/<Name>
    ns1__GetNodesResponse->PTZNode[i].Name = soap_strdup(soap, "PTZ");

    // <PTZNode>/<SupportedPTZSpaces>
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces = soap_new_tt__PTZSpaces(soap, -1);
    soap_default_tt__PTZSpaces(soap, ns1__GetNodesResponse->PTZNode->SupportedPTZSpaces);
    // <PTZNode>/<SupportedPTZSpaces>/<AbsolutePanTiltPositionSpace>
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace = soap_new_tt__Space2DDescription(soap, -1);
    // <PTZNode>/<SupportedPTZSpaces>/<AbsolutePanTiltPositionSpace>/<URI>
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/Position/2D/Absolute");
    // <PTZNode>/<SupportedPTZSpaces>/<AbsolutePanTiltPositionSpace>/<XRange>
    int x_min = rk_param_get_int("ptz.pan_min", 0) / 180.0;
    int x_max = rk_param_get_int("ptz.pan_max", 0) / 180.0;
    int y_min = rk_param_get_int("ptz.tilt_min", 0) / 180.0;
    int y_max = rk_param_get_int("ptz.tilt_max", 0) / 180.0;
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange = soap_new_tt__FloatRange(soap, -1);
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Min = x_min;
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Max = x_max;
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange = soap_new_tt__FloatRange(soap, -1);
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Min = y_min;
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Max = y_max;
    
    // <PTZNode>/<SupportedPTZSpaces>/<AbsoluteZoomPositionSpace>
    // 暂不支持缩放
    
    // <PTZNode>/<SupportedPTZSpaces>/<RelativePanTiltTranslationSpace>
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace = soap_new_tt__Space2DDescription(soap, -1);
    // <PTZNode>/<SupportedPTZSpaces>/<RelativePanTiltTranslationSpace>/<URI>
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/Translation/2D/Relative");
    // <PTZNode>/<SupportedPTZSpaces>/<RelativePanTiltTranslationSpace>/<XRange>
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange = soap_new_tt__FloatRange(soap, -1);
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange->Min = x_min;
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange->Max = x_max;
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange = soap_new_tt__FloatRange(soap, -1);
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange->Min = y_min;
    ns1__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange->Max = y_max;

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

    return SOAP_OK;
}

/** Web service operation '__ns1__GetNode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetNode(struct soap* soap, struct _ns1__GetNode *ns1__GetNode, struct _ns1__GetNodeResponse *ns1__GetNodeResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__SetConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetConfiguration(struct soap* soap, struct _ns1__SetConfiguration *ns1__SetConfiguration, struct _ns1__SetConfigurationResponse *ns1__SetConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__GetConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetConfigurationOptions(struct soap* soap, struct _ns1__GetConfigurationOptions *ns1__GetConfigurationOptions, struct _ns1__GetConfigurationOptionsResponse *ns1__GetConfigurationOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__GotoHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__GotoHomePosition(struct soap* soap, struct _ns1__GotoHomePosition *ns1__GotoHomePosition, struct _ns1__GotoHomePositionResponse *ns1__GotoHomePositionResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__SetHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetHomePosition(struct soap* soap, struct _ns1__SetHomePosition *ns1__SetHomePosition, char **ns1__SetHomePositionResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__ContinuousMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__ContinuousMove(struct soap* soap, struct _ns1__ContinuousMove *ns1__ContinuousMove, struct _ns1__ContinuousMoveResponse *ns1__ContinuousMoveResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__RelativeMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__RelativeMove(struct soap* soap, struct _ns1__RelativeMove *ns1__RelativeMove, struct _ns1__RelativeMoveResponse *ns1__RelativeMoveResponse) {
    float delta_x = ns1__RelativeMove->Translation->PanTilt->x;
    float delta_y = ns1__RelativeMove->Translation->PanTilt->y;

    LOG_DEBUG("delta_x: %f, delta_y: %f", delta_x, delta_y);
    
    return SOAP_OK;
}

/** Web service operation '__ns1__SendAuxiliaryCommand' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__SendAuxiliaryCommand(struct soap* soap, struct _ns1__SendAuxiliaryCommand *ns1__SendAuxiliaryCommand, struct _ns1__SendAuxiliaryCommandResponse *ns1__SendAuxiliaryCommandResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__AbsoluteMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__AbsoluteMove(struct soap* soap, struct _ns1__AbsoluteMove *ns1__AbsoluteMove, struct _ns1__AbsoluteMoveResponse *ns1__AbsoluteMoveResponse) {
    return SOAP_OK;
}

/** Web service operation '__ns1__Stop' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __ns1__Stop(struct soap* soap, struct _ns1__StopRequest *ns1__StopRequest, struct _ns1__StopResponse *ns1__StopResponse) {
    return SOAP_OK;
}
