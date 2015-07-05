#ifndef TOAST_NOTIFIER_H
#define TOAST_NOTIFIER_H


#include <array>
#include <string>

#include <windows.h>
#include <sal.h>
#include <windows.data.xml.dom.h>
#include <windows.ui.notifications.h>
#include <wrl/client.h>

#include "HRESULTCheck.h"
#include "StringWrapper.h"
#include "ToastEventHandler.h"



class ToastNotifier final {
public:
    using string_type = std::wstring;

    using IXmlAttribute = ABI::Windows::Data::Xml::Dom::IXmlAttribute;
    using IXmlDocument = ABI::Windows::Data::Xml::Dom::IXmlDocument;
    using IXmlElement = ABI::Windows::Data::Xml::Dom::IXmlElement;
    using IXmlNamedNodeMap = ABI::Windows::Data::Xml::Dom::IXmlNamedNodeMap;
    using IXmlNode = ABI::Windows::Data::Xml::Dom::IXmlNode;
    using IXmlNodeList = ABI::Windows::Data::Xml::Dom::IXmlNodeList;
    using IXmlText = ABI::Windows::Data::Xml::Dom::IXmlText;

    using IToastNotification = ABI::Windows::UI::Notifications::IToastNotification;
    using IToastNotificationFactory = ABI::Windows::UI::Notifications::IToastNotificationFactory;
    using IToastNotificationManagerStatics = ABI::Windows::UI::Notifications::IToastNotificationManagerStatics;
    using IToastNotifier = ABI::Windows::UI::Notifications::IToastNotifier;
    using ToastTemplateType = ABI::Windows::UI::Notifications::ToastTemplateType;

    template<typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;


public:
    enum class Duration {
        Short,
        Long
    };


public:
    ToastNotifier(
        _In_ string_type appId,
        _In_ ComPtr<ToastEventHandlerBase> toastEventHandler
    );

    // select toast type
    void SetType(
        _In_ ToastTemplateType type
    );

    // set image path
    void SetImage(
        _In_ string_type path
    );

    // set text
    void SetText(
        _In_ string_type text1,
        _In_ string_type text2 = L"",
        _In_ string_type text3 = L""
    );

    // set duration to short or long
    void SetDuration(
        _In_ Duration duration
    );

    // set audio
    void SetAudio(
        _In_ string_type audio
    );

    // display toast on desktop
    void DisplayToast() const;


private:
    // create toast xml
    ComPtr<IXmlDocument> CreateToastXml(
        _In_ ComPtr<IToastNotificationManagerStatics> toastManager
    ) const;

    // create toast notification using toast xml
    ComPtr<IToastNotification> CreateToast(
        _In_ ComPtr<IToastNotificationManagerStatics> toastManager,
        _In_ ComPtr<IXmlDocument> toastXml
    ) const;

    // display toast on desktop
    void DisplayToast(
        _In_ ComPtr<IToastNotificationManagerStatics> toastManager,
        _In_ ComPtr<IToastNotification> toast
    ) const;


private:
    // set image path into toast xml
    void SetImage(
        _In_ ComPtr<IXmlDocument> toastXml
    ) const;

    // set text into toast xml
    void SetText(
        _In_ ComPtr<IXmlDocument> toastXml
    ) const;

    // set duration to toast xml
    void SetDuration(
        _In_ ComPtr<IXmlDocument> toastXml
    ) const;

    void SetAudio(
        _In_ ComPtr<IXmlDocument> toastXml
    ) const;

    void SetNodeValue(
        _In_ ComPtr<IXmlDocument> document,
        _In_ ComPtr<IXmlNode> node,
        _In_ string_type const& text
    ) const;

    ComPtr<IXmlNode> CreateNode(
        _In_ ComPtr<IXmlDocument> document,
        _In_ ComPtr<IXmlNode> parent,
        _In_ string_type const& name
    ) const;

    // create an attribute for a node
    void CreateAttribute(
        _In_ ComPtr<IXmlDocument> document,
        _In_ ComPtr<IXmlNode> node,
        _In_ string_type const& name,
        _In_ string_type const& value
    ) const;

    
private:
    string_type m_appId;

    ComPtr<ToastEventHandlerBase> m_toastEventHandler;

    ToastTemplateType m_toastType = ToastTemplateType::ToastTemplateType_ToastText01;
    string_type m_imagePath;
    std::array<string_type, 3> m_text;
    Duration m_duration = Duration::Short;
    string_type m_audio;
};


#endif // TOAST_NOTIFIER_H
