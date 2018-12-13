#include "ToastNotifier.h"

#include <utility>

#include "Global.h"


ToastNotifier::ToastNotifier(
    _In_ string_type appId,
    _In_ ComPtr<ToastEventHandlerBase> toastEventHandler
) :
    m_appId(appId),
    m_toastEventHandler(toastEventHandler)
{}


void ToastNotifier::SetType(
    _In_ ToastTemplateType type
)
{
    m_toastType = type;
}


void ToastNotifier::SetImage(
    _In_ string_type path
)
{
    m_imagePath = std::move(path);
}


void ToastNotifier::SetText(
    _In_ string_type text1,
    _In_ string_type text2,
    _In_ string_type text3
)
{
    m_text[0] = std::move(text1);
    m_text[1] = std::move(text2);
    m_text[2] = std::move(text3);
}


void ToastNotifier::SetDuration(
    _In_ Duration duration
)
{
    m_duration = duration;
}


void ToastNotifier::SetAudio(
    _In_ string_type audio
)
{
    m_audio = audio;
}


void ToastNotifier::DisplayToast() const
{
    //
    // to display a toast notification:
    //     1. create xml
    //     2. create toast using created xml
    //     3. display toast
    //
    // read more at:
    //     https://msdn.microsoft.com/en-us/library/windows/apps/hh465448.aspx
    //

    using Windows::Foundation::GetActivationFactory;

    ComPtr<IToastNotificationManagerStatics> toastManager;
    HRESULTCheck hr = GetActivationFactory(
        StringReferenceWrapper{ RuntimeClass_Windows_UI_Notifications_ToastNotificationManager }.Get(),
        &toastManager
    );

    if (toastManager) {
        if (auto toastXml = CreateToastXml(toastManager)) {
            if (auto toast = CreateToast(toastManager, toastXml))
                DisplayToast(toastManager, toast);
        }
    }
}


ToastNotifier::ComPtr<ToastNotifier::IXmlDocument> ToastNotifier::CreateToastXml(
    _In_ ComPtr<IToastNotificationManagerStatics> toastManager
) const
{
    // get toast xml skeleton
    //
    // read more at:
    //     https://msdn.microsoft.com/en-us/library/windows/apps/hh761494.aspx
    //
    ComPtr<IXmlDocument> toastXml;
    HRESULTCheck hr = toastManager->GetTemplateContent(
        m_toastType,
        &toastXml
    );

    // set text to xml
    SetText(toastXml);

    // set image path to xml
    switch (m_toastType) {
    case ToastTemplateType::ToastTemplateType_ToastImageAndText01:
    case ToastTemplateType::ToastTemplateType_ToastImageAndText02:
    case ToastTemplateType::ToastTemplateType_ToastImageAndText03:
    case ToastTemplateType::ToastTemplateType_ToastImageAndText04:
        SetImage(toastXml);
        break;

    default:
        break;
    }

    SetDuration(toastXml);

    SetAudio(toastXml);

    return toastXml;
}


ToastNotifier::ComPtr<ToastNotifier::IToastNotification> ToastNotifier::CreateToast(
    _In_ ComPtr<IToastNotificationManagerStatics> toastManager,
    _In_ ComPtr<IXmlDocument> toastXml
) const
{
    using Windows::Foundation::GetActivationFactory;

    ComPtr<IToastNotificationFactory> factory;
    HRESULTCheck hr = GetActivationFactory(
        StringReferenceWrapper{ RuntimeClass_Windows_UI_Notifications_ToastNotification }.Get(),
        &factory
    );

    ComPtr<IToastNotification> toast;
    hr = factory->CreateToastNotification(toastXml.Get(), &toast);

    if (m_toastEventHandler) {
        EventRegistrationToken activatedToken, dismissedToken, failedToken;
        hr = toast->add_Activated(m_toastEventHandler.Get(), &activatedToken);
        hr = toast->add_Dismissed(m_toastEventHandler.Get(), &dismissedToken);
        hr = toast->add_Failed(m_toastEventHandler.Get(), &failedToken);
    }

    return toast;
}


void ToastNotifier::DisplayToast(
    _In_ ComPtr<IToastNotificationManagerStatics> toastManager,
    _In_ ComPtr<IToastNotification> toast
) const
{
    ComPtr<IToastNotifier> notifier;
    HRESULTCheck hr = toastManager->CreateToastNotifierWithId(
        StringWrapper{ m_appId }.Get(),
        &notifier
    );
    hr = notifier->Show(toast.Get());
}


void ToastNotifier::SetImage(
    _In_ ComPtr<IXmlDocument> toastXml
) const
{
    ComPtr<IXmlNodeList> imageNodes;
    HRESULTCheck hr = toastXml->GetElementsByTagName(
        StringReferenceWrapper{ L"image" }.Get(),
        &imageNodes
    );

    ComPtr<IXmlNode> node;
    hr = imageNodes->Item(0, &node);

    ComPtr<IXmlNamedNodeMap> attributes;
    hr = node->get_Attributes(&attributes);

    ComPtr<IXmlNode> srcAttribute;
    hr = attributes->GetNamedItem(StringReferenceWrapper{ L"src" }.Get(), &srcAttribute);
    
    SetNodeValue(toastXml, srcAttribute, m_imagePath);
}


void ToastNotifier::SetText(
    _In_ ComPtr<IXmlDocument> toastXml
) const
{
    ComPtr<IXmlNodeList> textNodes;
    HRESULTCheck hr = toastXml->GetElementsByTagName(
        StringReferenceWrapper{ L"text" }.Get(),
        &textNodes
    );

    UINT32 length = 0;
    hr = textNodes->get_Length(&length);
    for (UINT32 i = 0; i < length; ++i) {
        ComPtr<IXmlNode> node;
        hr = textNodes->Item(i, &node);

        SetNodeValue(toastXml, node, m_text[i]);
    }
}


void ToastNotifier::SetDuration(
    _In_ ComPtr<IXmlDocument> toastXml
) const
{
    ComPtr<IXmlNodeList> toastNodes;
    HRESULTCheck hr = toastXml->GetElementsByTagName(
        StringReferenceWrapper{ L"toast" }.Get(),
        &toastNodes
    );

    ComPtr<IXmlNode> node;
    hr = toastNodes->Item(0, &node);

    switch (m_duration) {
    case Duration::Short:
        CreateAttribute(toastXml, node, L"duration", L"short");
        break;

    case Duration::Long:
        CreateAttribute(toastXml, node, L"duration", L"long");
        break;
    }
}


void ToastNotifier::SetAudio(
    _In_ ComPtr<IXmlDocument> toastXml
) const
{
    //
    // read more at:
    //     https://msdn.microsoft.com/en-us/library/windows/apps/hh761492.aspx
    //

    ComPtr<IXmlNodeList> toastNodes;
    HRESULTCheck hr = toastXml->GetElementsByTagName(
        StringReferenceWrapper{ L"toast" }.Get(),
        &toastNodes
    );

    ComPtr<IXmlNode> toastNode;
    hr = toastNodes->Item(0, &toastNode);

    auto audioNode = CreateNode(toastXml, toastNode, L"audio");

    CreateAttribute(toastXml, audioNode, L"src", m_audio);
}


void ToastNotifier::SetNodeValue(
    _In_ ComPtr<IXmlDocument> document,
    _In_ ComPtr<IXmlNode> node,
    _In_ string_type const& text
) const
{
    ComPtr<IXmlText> inputText;
    HRESULTCheck hr = document->CreateTextNode(
        StringWrapper{ text.c_str() }.Get(),
        &inputText
    );
        
    ComPtr<IXmlNode> inputTextNode;
    hr = inputText.As(&inputTextNode);

    ComPtr<IXmlNode> appendedNode;
    hr = node->AppendChild(
        inputTextNode.Get(),
        &appendedNode
    );
}


ToastNotifier::ComPtr<ToastNotifier::IXmlNode> ToastNotifier::CreateNode(
    _In_ ComPtr<IXmlDocument> document,
    _In_ ComPtr<IXmlNode> parent,
    _In_ string_type const& name
) const
{
    ComPtr<IXmlElement> element;
    HRESULTCheck hr = document->CreateElement(StringWrapper{ name }.Get(), &element);
    
    ComPtr<IXmlNode> newNode;
    hr = element.As(&newNode);

    ComPtr<IXmlNode> appendedNode;
    hr = parent->AppendChild(newNode.Get(), &appendedNode);

    return appendedNode;
}


void ToastNotifier::CreateAttribute(
    _In_ ComPtr<IXmlDocument> document,
    _In_ ComPtr<IXmlNode> node,
    _In_ string_type const& name,
    _In_ string_type const& value
) const
{
    ComPtr<IXmlNamedNodeMap> attributes;
    HRESULTCheck hr = node->get_Attributes(&attributes);

    ComPtr<IXmlAttribute> attr;
    hr = document->CreateAttribute(StringWrapper{ name }.Get(), &attr);
    hr = attr->put_Value(StringWrapper{ value }.Get());

    ComPtr<IXmlNode> attrNode;
    hr = attr.As(&attrNode);

    ComPtr<IXmlNode> previousNode;
    hr = attributes->SetNamedItem(attrNode.Get(), &previousNode);
}
