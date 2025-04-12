#include "soapH.h"
#include "soapStub.h"

#include "global.h"
#include "onvif_config.h"

/** Web service operation '__tds__GetServices' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices(struct soap* soap, struct _tds__GetServices *tds__GetServices, struct _tds__GetServicesResponse *tds__GetServicesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServiceCapabilities(struct soap* soap, struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities, struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetDeviceInformation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDeviceInformation(struct soap* soap, struct _tds__GetDeviceInformation *tds__GetDeviceInformation, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse) {
    LOG_DEBUG("__tds__GetDeviceInformation");

    tds__GetDeviceInformationResponse->Manufacturer = soap_strdup(soap, "CQUPT-YF403");
    tds__GetDeviceInformationResponse->Model = soap_strdup(soap, "RV1106-IPCTERMINAL");
    tds__GetDeviceInformationResponse->FirmwareVersion = soap_strdup(soap, "V1.0.0");
    tds__GetDeviceInformationResponse->SerialNumber = soap_strdup(soap, "IPC0000001");
    tds__GetDeviceInformationResponse->HardwareId = soap_strdup(soap, "RV1106");

    return SOAP_OK;
}

/** Web service operation '__tds__SetSystemDateAndTime' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemDateAndTime(struct soap* soap, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemDateAndTime' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemDateAndTime(struct soap* soap, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetSystemFactoryDefault' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault(struct soap* soap, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__UpgradeSystemFirmware' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware(struct soap* soap, struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SystemReboot' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot(struct soap* soap, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__RestoreSystem' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RestoreSystem(struct soap* soap, struct _tds__RestoreSystem *tds__RestoreSystem, struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemBackup' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup(struct soap* soap, struct _tds__GetSystemBackup *tds__GetSystemBackup, struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemLog' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog(struct soap* soap, struct _tds__GetSystemLog *tds__GetSystemLog, struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemSupportInformation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation(struct soap* soap, struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes(struct soap* soap, struct _tds__GetScopes *tds__GetScopes, struct _tds__GetScopesResponse *tds__GetScopesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes(struct soap* soap, struct _tds__SetScopes *tds__SetScopes, struct _tds__SetScopesResponse *tds__SetScopesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__AddScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes(struct soap* soap, struct _tds__AddScopes *tds__AddScopes, struct _tds__AddScopesResponse *tds__AddScopesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__RemoveScopes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes(struct soap* soap, struct _tds__RemoveScopes *tds__RemoveScopes, struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode(struct soap* soap, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode(struct soap* soap, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode, struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetRemoteDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteDiscoveryMode(struct soap* soap, struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetRemoteDiscoveryMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteDiscoveryMode(struct soap* soap, struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetDPAddresses' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDPAddresses(struct soap* soap, struct _tds__GetDPAddresses *tds__GetDPAddresses, struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetEndpointReference' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetEndpointReference(struct soap* soap, struct _tds__GetEndpointReference *tds__GetEndpointReference, struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetRemoteUser' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteUser(struct soap* soap, struct _tds__GetRemoteUser *tds__GetRemoteUser, struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetRemoteUser' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser(struct soap* soap, struct _tds__SetRemoteUser *tds__SetRemoteUser, struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetUsers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers(struct soap* soap, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__CreateUsers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers(struct soap* soap, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteUsers' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers(struct soap* soap, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetUser' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser(struct soap* soap, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetWsdlUrl' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetWsdlUrl(struct soap* soap, struct _tds__GetWsdlUrl *tds__GetWsdlUrl, struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetPasswordComplexityOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordComplexityOptions(struct soap* soap, struct _tds__GetPasswordComplexityOptions *tds__GetPasswordComplexityOptions, struct _tds__GetPasswordComplexityOptionsResponse *tds__GetPasswordComplexityOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetPasswordComplexityConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordComplexityConfiguration(struct soap* soap, struct _tds__GetPasswordComplexityConfiguration *tds__GetPasswordComplexityConfiguration, struct _tds__GetPasswordComplexityConfigurationResponse *tds__GetPasswordComplexityConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetPasswordComplexityConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetPasswordComplexityConfiguration(struct soap* soap, struct _tds__SetPasswordComplexityConfiguration *tds__SetPasswordComplexityConfiguration, struct _tds__SetPasswordComplexityConfigurationResponse *tds__SetPasswordComplexityConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetPasswordHistoryConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordHistoryConfiguration(struct soap* soap, struct _tds__GetPasswordHistoryConfiguration *tds__GetPasswordHistoryConfiguration, struct _tds__GetPasswordHistoryConfigurationResponse *tds__GetPasswordHistoryConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetPasswordHistoryConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetPasswordHistoryConfiguration(struct soap* soap, struct _tds__SetPasswordHistoryConfiguration *tds__SetPasswordHistoryConfiguration, struct _tds__SetPasswordHistoryConfigurationResponse *tds__SetPasswordHistoryConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetAuthFailureWarningOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAuthFailureWarningOptions(struct soap* soap, struct _tds__GetAuthFailureWarningOptions *tds__GetAuthFailureWarningOptions, struct _tds__GetAuthFailureWarningOptionsResponse *tds__GetAuthFailureWarningOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetAuthFailureWarningConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAuthFailureWarningConfiguration(struct soap* soap, struct _tds__GetAuthFailureWarningConfiguration *tds__GetAuthFailureWarningConfiguration, struct _tds__GetAuthFailureWarningConfigurationResponse *tds__GetAuthFailureWarningConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetAuthFailureWarningConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAuthFailureWarningConfiguration(struct soap* soap, struct _tds__SetAuthFailureWarningConfiguration *tds__SetAuthFailureWarningConfiguration, struct _tds__SetAuthFailureWarningConfigurationResponse *tds__SetAuthFailureWarningConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities(struct soap* soap, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse) {
    LOG_DEBUG("__tds__GetCapabilities");
    
	char ipaddr[IP_ADDR_LENGTH] = { 0 };

    // 获取本机IP地址
    if (get_local_ip("eth0", ipaddr) < 0) {
        LOG_ERROR("Get local IP address failed\n");
        return SOAP_ERR;
    }

    // <Capabilities>
    tds__GetCapabilitiesResponse->Capabilities = (struct tt__Capabilities *)soap_malloc(soap, sizeof(struct tt__Capabilities));
    memset(tds__GetCapabilitiesResponse->Capabilities, 0, sizeof(struct tt__Capabilities));

    // <Capabilities>/<Device>
    if (tds__GetCapabilities->Category[0] == tt__CapabilityCategory__Device ||
        tds__GetCapabilities->Category[0] == tt__CapabilityCategory__All) {
        // <Device>
        tds__GetCapabilitiesResponse->Capabilities->Device = (struct tt__DeviceCapabilities *)soap_malloc(soap, sizeof(struct tt__DeviceCapabilities));
        memset(tds__GetCapabilitiesResponse->Capabilities->Device, 0, sizeof(struct tt__DeviceCapabilities));
        
        // <Device>/<XAddr>
        tds__GetCapabilitiesResponse->Capabilities->Device->XAddr = (char *)soap_malloc(soap, sizeof(char) * 100 );
        memset(tds__GetCapabilitiesResponse->Capabilities->Device->XAddr, 0, sizeof(char) * 100);
        sprintf(tds__GetCapabilitiesResponse->Capabilities->Device->XAddr, "http://%s:%d/onvif/device_service", ipaddr, WEB_SERVICE_PORT);
        
        // <Device>/<Network>
        tds__GetCapabilitiesResponse->Capabilities->Device->Network = (struct tt__NetworkCapabilities *)soap_malloc(soap, sizeof(struct tt__NetworkCapabilities ));
        memset(tds__GetCapabilitiesResponse->Capabilities->Device->Network, 0, sizeof(struct tt__NetworkCapabilities ));
        tds__GetCapabilitiesResponse->Capabilities->Device->Network->IPFilter = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Device->Network->IPFilter) = xsd__boolean__false_;                //关闭功能 xsd__boolean__true_
        tds__GetCapabilitiesResponse->Capabilities->Device->Network->ZeroConfiguration= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Device->Network->ZeroConfiguration) = xsd__boolean__false_;       //打开功能 xsd__boolean__false_
        tds__GetCapabilitiesResponse->Capabilities->Device->Network->IPVersion6 = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Device->Network->IPVersion6) = xsd__boolean__false_;              //关闭功能 xsd__boolean__true_
        tds__GetCapabilitiesResponse->Capabilities->Device->Network->DynDNS = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Device->Network->DynDNS) = xsd__boolean__false_;                  //打开功能 xsd__boolean__false_
        tds__GetCapabilitiesResponse->Capabilities->Device->Network->Extension = (struct tt__NetworkCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__NetworkCapabilitiesExtension));
        memset(tds__GetCapabilitiesResponse->Capabilities->Device->Network->Extension, 0, sizeof(struct tt__NetworkCapabilitiesExtension ));
        tds__GetCapabilitiesResponse->Capabilities->Device->Network->Extension->__size = 1;
        tds__GetCapabilitiesResponse->Capabilities->Device->Network->Extension->Dot11Configuration = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Device->Network->Extension->Dot11Configuration) = xsd__boolean__false_;

        // <Device>/<System>
        tds__GetCapabilitiesResponse->Capabilities->Device->System = (struct tt__SystemCapabilities *)soap_malloc(soap, sizeof(struct tt__SystemCapabilities));
        memset( tds__GetCapabilitiesResponse->Capabilities->Device->System, 0, sizeof(struct tt__SystemCapabilities));
        tds__GetCapabilitiesResponse->Capabilities->Device->System->DiscoveryResolve = xsd__boolean__true_;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->DiscoveryBye     = xsd__boolean__true_;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->RemoteDiscovery  = xsd__boolean__true_;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->SystemBackup     = xsd__boolean__true_;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->SystemLogging    = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->FirmwareUpgrade  = xsd__boolean__true_;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->__sizeSupportedVersions = 1;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->SupportedVersions = (struct tt__OnvifVersion *)soap_malloc(soap, sizeof(struct tt__OnvifVersion));
        tds__GetCapabilitiesResponse->Capabilities->Device->System->SupportedVersions->Major = 1;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->SupportedVersions->Minor = 10;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->Extension = (struct tt__SystemCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__SystemCapabilitiesExtension));
        memset( tds__GetCapabilitiesResponse->Capabilities->Device->System->Extension, 0, sizeof(struct tt__SystemCapabilitiesExtension));
        tds__GetCapabilitiesResponse->Capabilities->Device->System->Extension->HttpFirmwareUpgrade = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Device->System->Extension->HttpFirmwareUpgrade) = xsd__boolean__true_;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->Extension->HttpSystemBackup = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Device->System->Extension->HttpSystemBackup) = xsd__boolean__true_;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->Extension->HttpSystemLogging = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Device->System->Extension->HttpSystemLogging) = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->System->Extension->HttpSupportInformation = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Device->System->Extension->HttpSupportInformation) = xsd__boolean__true_;

        // <Device>/<Security>
        tds__GetCapabilitiesResponse->Capabilities->Device->Security = (struct tt__SecurityCapabilities *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilities));
        memset(tds__GetCapabilitiesResponse->Capabilities->Device->Security, 0, sizeof(struct tt__SecurityCapabilities));
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->TLS1_x002e1          = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->TLS1_x002e2          = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->OnboardKeyGeneration = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->AccessPolicyConfig   = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->X_x002e509Token      = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->SAMLToken            = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->KerberosToken        = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->RELToken             = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->Extension = (struct tt__SecurityCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilitiesExtension));
        memset(tds__GetCapabilitiesResponse->Capabilities->Device->Security->Extension, 0, sizeof(struct tt__SecurityCapabilitiesExtension));
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->Extension->Extension =
                (struct tt__SecurityCapabilitiesExtension2 *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilitiesExtension2));
        memset(tds__GetCapabilitiesResponse->Capabilities->Device->Security->Extension->Extension, 0, sizeof(struct tt__SecurityCapabilitiesExtension2));
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->Extension->Extension->Dot1X = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Device->Security->Extension->Extension->RemoteUserHandling = xsd__boolean__false_;
    }

    // <Capabilities>/<Events>
    if (tds__GetCapabilities->Category[0] == tt__CapabilityCategory__Events ||
        tds__GetCapabilities->Category[0] == tt__CapabilityCategory__All) {
        // <Events>
        tds__GetCapabilitiesResponse->Capabilities->Events = (struct tt__EventCapabilities *)soap_malloc(soap, sizeof(struct tt__EventCapabilities));
        memset(tds__GetCapabilitiesResponse->Capabilities->Events, 0, sizeof(struct tt__EventCapabilities));
        
        // <Events>/<XAddr>
        tds__GetCapabilitiesResponse->Capabilities->Events->XAddr = (char *)soap_malloc(soap, sizeof(char) * 100 );
        memset(tds__GetCapabilitiesResponse->Capabilities->Events->XAddr, 0, sizeof(char) * 100);
        sprintf(tds__GetCapabilitiesResponse->Capabilities->Events->XAddr, "http://%s:%d/onvif/event_service", ipaddr, WEB_SERVICE_PORT);

        // <Events>/<WSSubscriptionPolicySupport>
        tds__GetCapabilitiesResponse->Capabilities->Events->WSSubscriptionPolicySupport = xsd__boolean__false_;

        // <Events>/<WSPullPointSupport>
        tds__GetCapabilitiesResponse->Capabilities->Events->WSPullPointSupport = xsd__boolean__false_;

        // <Events>/<WSPausableSubscriptionManagerInterfaceSupport>
        tds__GetCapabilitiesResponse->Capabilities->Events->WSPausableSubscriptionManagerInterfaceSupport = xsd__boolean__false_;
    }
    
    // <Capabilities>/<Media>
    if (tds__GetCapabilities->Category[0] == tt__CapabilityCategory__Media ||
        tds__GetCapabilities->Category[0] == tt__CapabilityCategory__All) {
        // <Media>
        tds__GetCapabilitiesResponse->Capabilities->Media = (struct tt__MediaCapabilities *)soap_malloc(soap, sizeof(struct tt__MediaCapabilities));
        memset(tds__GetCapabilitiesResponse->Capabilities->Media, 0, sizeof(struct tt__MediaCapabilities));
        
        // <Media>/<XAddr>
        tds__GetCapabilitiesResponse->Capabilities->Media->XAddr = (char *)soap_malloc(soap, sizeof(char) * 100 );
        memset(tds__GetCapabilitiesResponse->Capabilities->Media->XAddr, '\0', sizeof(char) * 100);
        sprintf(tds__GetCapabilitiesResponse->Capabilities->Media->XAddr, "http://%s:%d/onvif/media_service", ipaddr, WEB_SERVICE_PORT);
        
        // <Media>/<StreamingCapabilities>
        tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities = (struct tt__RealTimeStreamingCapabilities *)soap_malloc(soap, sizeof(struct tt__RealTimeStreamingCapabilities));
        memset(tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities, 0, sizeof(struct tt__RealTimeStreamingCapabilities));
        tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTPMulticast = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTPMulticast) = xsd__boolean__false_;
        tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP) = xsd__boolean__true_;
        tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTP_USCORETCP = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
        *(tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTP_USCORETCP) = xsd__boolean__true_;
    }

    // <Capabilities>/<PTZ>
    if (tds__GetCapabilities->Category[0] == tt__CapabilityCategory__PTZ ||
        tds__GetCapabilities->Category[0] == tt__CapabilityCategory__All) {
        // <PTZ>
        tds__GetCapabilitiesResponse->Capabilities->PTZ = (struct tt__PTZCapabilities *)soap_malloc(soap, sizeof(struct tt__PTZCapabilities));
        memset(tds__GetCapabilitiesResponse->Capabilities->PTZ, 0, sizeof(struct tt__PTZCapabilities));
        
        // <PTZ>/<XAddr>
        tds__GetCapabilitiesResponse->Capabilities->PTZ->XAddr = (char *)soap_malloc(soap, sizeof(char) * 100 );
        memset(tds__GetCapabilitiesResponse->Capabilities->PTZ->XAddr, 0, sizeof(char) * 100);
        sprintf(tds__GetCapabilitiesResponse->Capabilities->PTZ->XAddr, "http://%s:%d/onvif/ptz_service", ipaddr, WEB_SERVICE_PORT);
    }

    return SOAP_OK;
}

/** Web service operation '__tds__SetDPAddresses' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDPAddresses(struct soap* soap, struct _tds__SetDPAddresses *tds__SetDPAddresses, struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetHostname' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetHostname(struct soap* soap, struct _tds__GetHostname *tds__GetHostname, struct _tds__GetHostnameResponse *tds__GetHostnameResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetHostname' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostname(struct soap* soap, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetHostnameFromDHCP' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostnameFromDHCP(struct soap* soap, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS(struct soap* soap, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS(struct soap* soap, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetNTP' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP(struct soap* soap, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetNTP' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP(struct soap* soap, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetDynamicDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS(struct soap* soap, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetDynamicDNS' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS(struct soap* soap, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetNetworkInterfaces' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces(struct soap* soap, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetNetworkInterfaces' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces(struct soap* soap, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetNetworkProtocols' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols(struct soap* soap, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetNetworkProtocols' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols(struct soap* soap, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetNetworkDefaultGateway' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway(struct soap* soap, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetNetworkDefaultGateway' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway(struct soap* soap, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetZeroConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration(struct soap* soap, struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration, struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetZeroConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration(struct soap* soap, struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter(struct soap* soap, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter(struct soap* soap, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__AddIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter(struct soap* soap, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__RemoveIPAddressFilter' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter(struct soap* soap, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetAccessPolicy' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy(struct soap* soap, struct _tds__GetAccessPolicy *tds__GetAccessPolicy, struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetAccessPolicy' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy(struct soap* soap, struct _tds__SetAccessPolicy *tds__SetAccessPolicy, struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__CreateCertificate' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateCertificate(struct soap* soap, struct _tds__CreateCertificate *tds__CreateCertificate, struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetCertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificates(struct soap* soap, struct _tds__GetCertificates *tds__GetCertificates, struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetCertificatesStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificatesStatus(struct soap* soap, struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus, struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetCertificatesStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetCertificatesStatus(struct soap* soap, struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus, struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteCertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteCertificates(struct soap* soap, struct _tds__DeleteCertificates *tds__DeleteCertificates, struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetPkcs10Request' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPkcs10Request(struct soap* soap, struct _tds__GetPkcs10Request *tds__GetPkcs10Request, struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__LoadCertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificates(struct soap* soap, struct _tds__LoadCertificates *tds__LoadCertificates, struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetClientCertificateMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetClientCertificateMode(struct soap* soap, struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode, struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetClientCertificateMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetClientCertificateMode(struct soap* soap, struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode, struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetRelayOutputs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRelayOutputs(struct soap* soap, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetRelayOutputSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings(struct soap* soap, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetRelayOutputState' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState(struct soap* soap, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SendAuxiliaryCommand' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand(struct soap* soap, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetCACertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCACertificates(struct soap* soap, struct _tds__GetCACertificates *tds__GetCACertificates, struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__LoadCertificateWithPrivateKey' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificateWithPrivateKey(struct soap* soap, struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetCertificateInformation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificateInformation(struct soap* soap, struct _tds__GetCertificateInformation *tds__GetCertificateInformation, struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__LoadCACertificates' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCACertificates(struct soap* soap, struct _tds__LoadCACertificates *tds__LoadCACertificates, struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__CreateDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateDot1XConfiguration(struct soap* soap, struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDot1XConfiguration(struct soap* soap, struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfiguration(struct soap* soap, struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetDot1XConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfigurations(struct soap* soap, struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteDot1XConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteDot1XConfiguration(struct soap* soap, struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetDot11Capabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Capabilities(struct soap* soap, struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities, struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetDot11Status' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status(struct soap* soap, struct _tds__GetDot11Status *tds__GetDot11Status, struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__ScanAvailableDot11Networks' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks(struct soap* soap, struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemUris' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris(struct soap* soap, struct _tds__GetSystemUris *tds__GetSystemUris, struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__StartFirmwareUpgrade' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade(struct soap* soap, struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__StartSystemRestore' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore(struct soap* soap, struct _tds__StartSystemRestore *tds__StartSystemRestore, struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetStorageConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfigurations(struct soap* soap, struct _tds__GetStorageConfigurations *tds__GetStorageConfigurations, struct _tds__GetStorageConfigurationsResponse *tds__GetStorageConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__CreateStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateStorageConfiguration(struct soap* soap, struct _tds__CreateStorageConfiguration *tds__CreateStorageConfiguration, struct _tds__CreateStorageConfigurationResponse *tds__CreateStorageConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfiguration(struct soap* soap, struct _tds__GetStorageConfiguration *tds__GetStorageConfiguration, struct _tds__GetStorageConfigurationResponse *tds__GetStorageConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetStorageConfiguration(struct soap* soap, struct _tds__SetStorageConfiguration *tds__SetStorageConfiguration, struct _tds__SetStorageConfigurationResponse *tds__SetStorageConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteStorageConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteStorageConfiguration(struct soap* soap, struct _tds__DeleteStorageConfiguration *tds__DeleteStorageConfiguration, struct _tds__DeleteStorageConfigurationResponse *tds__DeleteStorageConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__GetGeoLocation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetGeoLocation(struct soap* soap, struct _tds__GetGeoLocation *tds__GetGeoLocation, struct _tds__GetGeoLocationResponse *tds__GetGeoLocationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetGeoLocation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetGeoLocation(struct soap* soap, struct _tds__SetGeoLocation *tds__SetGeoLocation, struct _tds__SetGeoLocationResponse *tds__SetGeoLocationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteGeoLocation' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteGeoLocation(struct soap* soap, struct _tds__DeleteGeoLocation *tds__DeleteGeoLocation, struct _tds__DeleteGeoLocationResponse *tds__DeleteGeoLocationResponse) {
    return SOAP_OK;
}

/** Web service operation '__tds__SetHashingAlgorithm' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHashingAlgorithm(struct soap* soap, struct _tds__SetHashingAlgorithm *tds__SetHashingAlgorithm, struct _tds__SetHashingAlgorithmResponse *tds__SetHashingAlgorithmResponse) {
    return SOAP_OK;
}
