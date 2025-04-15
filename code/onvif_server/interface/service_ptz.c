#include "soapH.h"
#include "soapStub.h"

#include <time.h>

#include "global.h"
#include "helper_ptz.h"

/** Web service operation '__tptz__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetServiceCapabilities(struct soap* soap, struct _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, struct _tptz__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GetConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurations(struct soap* soap, struct _tptz__GetConfigurations *tptz__GetConfigurations, struct _tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse) {
    LOG_DEBUG("__tptz__GetConfigurations\n");
    // <PTZConfigurations>/<__sizePTZConfigurations>
    tptz__GetConfigurationsResponse->__sizePTZConfiguration = 1;
    
    // <PTZConfigurations>/<PTZConfiguration>
    tptz__GetConfigurationsResponse->PTZConfiguration = helper_create_default_ptz_configuration(soap);

    return SOAP_OK;
}

/** Web service operation '__tptz__GetPresets' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresets(struct soap* soap, struct _tptz__GetPresets *tptz__GetPresets, struct _tptz__GetPresetsResponse *tptz__GetPresetsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__SetPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetPreset(struct soap* soap, struct _tptz__SetPreset *tptz__SetPreset, struct _tptz__SetPresetResponse *tptz__SetPresetResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__RemovePreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePreset(struct soap* soap, struct _tptz__RemovePreset *tptz__RemovePreset, struct _tptz__RemovePresetResponse *tptz__RemovePresetResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GotoPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoPreset(struct soap* soap, struct _tptz__GotoPreset *tptz__GotoPreset, struct _tptz__GotoPresetResponse *tptz__GotoPresetResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GetStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetStatus(struct soap* soap, struct _tptz__GetStatus *tptz__GetStatus, struct _tptz__GetStatusResponse *tptz__GetStatusResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GetConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfiguration(struct soap* soap, struct _tptz__GetConfiguration *tptz__GetConfiguration, struct _tptz__GetConfigurationResponse *tptz__GetConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GetNodes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNodes(struct soap* soap, struct _tptz__GetNodes *tptz__GetNodes, struct _tptz__GetNodesResponse *tptz__GetNodesResponse) {
    LOG_DEBUG("__tptz__GetNodes\n");

    // Node 个数
    tptz__GetNodesResponse->__sizePTZNode = 1;

    // <PTZNodes>
    tptz__GetNodesResponse->PTZNode = helper_create_ptz_node(soap, 0);

    return SOAP_OK;
}

/** Web service operation '__tptz__GetNode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNode(struct soap* soap, struct _tptz__GetNode *tptz__GetNode, struct _tptz__GetNodeResponse *tptz__GetNodeResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__SetConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetConfiguration(struct soap* soap, struct _tptz__SetConfiguration *tptz__SetConfiguration, struct _tptz__SetConfigurationResponse *tptz__SetConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GetConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurationOptions(struct soap* soap, struct _tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, struct _tptz__GetConfigurationOptionsResponse *tptz__GetConfigurationOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GotoHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoHomePosition(struct soap* soap, struct _tptz__GotoHomePosition *tptz__GotoHomePosition, struct _tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__SetHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetHomePosition(struct soap* soap, struct _tptz__SetHomePosition *tptz__SetHomePosition, struct _tptz__SetHomePositionResponse *tptz__SetHomePositionResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__ContinuousMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__ContinuousMove(struct soap* soap, struct _tptz__ContinuousMove *tptz__ContinuousMove, struct _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__RelativeMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RelativeMove(struct soap* soap, struct _tptz__RelativeMove *tptz__RelativeMove, struct _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse) {
    LOG_DEBUG("__tptz__RelativeMove\n");
    // 获取相对移动的参数
    float delta_x = tptz__RelativeMove->Translation->PanTilt->x;
    float delta_y = tptz__RelativeMove->Translation->PanTilt->y;

    // 执行相对移动操作
    helper_do_ptz_relative_move(delta_x, delta_y);
    
    return SOAP_OK;
}

/** Web service operation '__tptz__SendAuxiliaryCommand' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SendAuxiliaryCommand(struct soap* soap, struct _tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, struct _tptz__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__AbsoluteMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__AbsoluteMove(struct soap* soap, struct _tptz__AbsoluteMove *tptz__AbsoluteMove, struct _tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse) {
    LOG_DEBUG("__tptz__AbsoluteMove\n");
    // 获取绝对移动的参数
    float x = tptz__AbsoluteMove->Position->PanTilt->x;
    float y = tptz__AbsoluteMove->Position->PanTilt->y;

    // 执行绝对移动操作
    helper_do_ptz_absolute_move(x, y);
    
    return SOAP_OK;
}

/** Web service operation '__tptz__GeoMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GeoMove(struct soap* soap, struct _tptz__GeoMove *tptz__GeoMove, struct _tptz__GeoMoveResponse *tptz__GeoMoveResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__Stop' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__Stop(struct soap* soap, struct _tptz__Stop *tptz__Stop, struct _tptz__StopResponse *tptz__StopResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GetPresetTours' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTours(struct soap* soap, struct _tptz__GetPresetTours *tptz__GetPresetTours, struct _tptz__GetPresetToursResponse *tptz__GetPresetToursResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GetPresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTour(struct soap* soap, struct _tptz__GetPresetTour *tptz__GetPresetTour, struct _tptz__GetPresetTourResponse *tptz__GetPresetTourResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GetPresetTourOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTourOptions(struct soap* soap, struct _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, struct _tptz__GetPresetTourOptionsResponse *tptz__GetPresetTourOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__CreatePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__CreatePresetTour(struct soap* soap, struct _tptz__CreatePresetTour *tptz__CreatePresetTour, struct _tptz__CreatePresetTourResponse *tptz__CreatePresetTourResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__ModifyPresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__ModifyPresetTour(struct soap* soap, struct _tptz__ModifyPresetTour *tptz__ModifyPresetTour, struct _tptz__ModifyPresetTourResponse *tptz__ModifyPresetTourResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__OperatePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__OperatePresetTour(struct soap* soap, struct _tptz__OperatePresetTour *tptz__OperatePresetTour, struct _tptz__OperatePresetTourResponse *tptz__OperatePresetTourResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__RemovePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePresetTour(struct soap* soap, struct _tptz__RemovePresetTour *tptz__RemovePresetTour, struct _tptz__RemovePresetTourResponse *tptz__RemovePresetTourResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__GetCompatibleConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetCompatibleConfigurations(struct soap* soap, struct _tptz__GetCompatibleConfigurations *tptz__GetCompatibleConfigurations, struct _tptz__GetCompatibleConfigurationsResponse *tptz__GetCompatibleConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tptz__MoveAndStartTracking' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__MoveAndStartTracking(struct soap* soap, struct _tptz__MoveAndStartTracking *tptz__MoveAndStartTracking, struct _tptz__MoveAndStartTrackingResponse *tptz__MoveAndStartTrackingResponse) {
    return SOAP_OK;
}


