// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "AISettingsViewModel.h"
#include "AISettingsViewModel.g.cpp"
#include "EnumEntry.h"

#include <LibraryResources.h>
#include <WtExeUtils.h>
#include <shellapi.h>

using namespace winrt;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Microsoft::Terminal::Settings::Model;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Microsoft::Terminal::Settings::Editor::implementation
{
    AISettingsViewModel::AISettingsViewModel(Model::CascadiaSettings settings) :
        _Settings{ settings }
    {
        _githubAuthCompleteRevoker = MainPage::GithubAuthCompleted(winrt::auto_revoke, { this, &AISettingsViewModel::_OnGithubAuthCompleted });
    }

    bool AISettingsViewModel::AreAzureOpenAIKeyAndEndpointSet()
    {
        return !_Settings.GlobalSettings().AIInfo().AzureOpenAIKey().empty() && !_Settings.GlobalSettings().AIInfo().AzureOpenAIEndpoint().empty();
    }

    winrt::hstring AISettingsViewModel::AzureOpenAIEndpoint()
    {
        return _Settings.GlobalSettings().AIInfo().AzureOpenAIEndpoint();
    }

    void AISettingsViewModel::AzureOpenAIEndpoint(winrt::hstring endpoint)
    {
        _Settings.GlobalSettings().AIInfo().AzureOpenAIEndpoint(endpoint);
        _NotifyChanges(L"AreAzureOpenAIKeyAndEndpointSet");
    }

    winrt::hstring AISettingsViewModel::AzureOpenAIKey()
    {
        return _Settings.GlobalSettings().AIInfo().AzureOpenAIKey();
    }

    void AISettingsViewModel::AzureOpenAIKey(winrt::hstring key)
    {
        _Settings.GlobalSettings().AIInfo().AzureOpenAIKey(key);
        _NotifyChanges(L"AreAzureOpenAIKeyAndEndpointSet");
    }

    bool AISettingsViewModel::IsOpenAIKeySet()
    {
        return !_Settings.GlobalSettings().AIInfo().OpenAIKey().empty();
    }

    winrt::hstring AISettingsViewModel::OpenAIKey()
    {
        return _Settings.GlobalSettings().AIInfo().OpenAIKey();
    }

    void AISettingsViewModel::OpenAIKey(winrt::hstring key)
    {
        _Settings.GlobalSettings().AIInfo().OpenAIKey(key);
        _NotifyChanges(L"IsOpenAIKeySet");
    }

    bool AISettingsViewModel::AzureOpenAIActive()
    {
        return _Settings.GlobalSettings().AIInfo().ActiveProvider() == Model::LLMProvider::AzureOpenAI;
    }

    void AISettingsViewModel::AzureOpenAIActive(bool active)
    {
        if (active)
        {
            _Settings.GlobalSettings().AIInfo().ActiveProvider(Model::LLMProvider::AzureOpenAI);
            _NotifyChanges(L"AzureOpenAIActive", L"OpenAIActive", L"GithubCopilotActive");
        }
    }

    bool AISettingsViewModel::OpenAIActive()
    {
        return _Settings.GlobalSettings().AIInfo().ActiveProvider() == Model::LLMProvider::OpenAI;
    }

    void AISettingsViewModel::OpenAIActive(bool active)
    {
        if (active)
        {
            _Settings.GlobalSettings().AIInfo().ActiveProvider(Model::LLMProvider::OpenAI);
            _NotifyChanges(L"AzureOpenAIActive", L"OpenAIActive", L"GithubCopilotActive");
        }
    }

    bool AISettingsViewModel::AreGithubCopilotTokensSet()
    {
        return !_Settings.GlobalSettings().AIInfo().GithubCopilotAuthToken().empty() && !_Settings.GlobalSettings().AIInfo().GithubCopilotRefreshToken().empty();
    }

    void AISettingsViewModel::GithubCopilotAuthToken(winrt::hstring authToken)
    {
        _Settings.GlobalSettings().AIInfo().GithubCopilotAuthToken(authToken);
        _NotifyChanges(L"AreGithubCopilotTokensSet");
    }

    void AISettingsViewModel::GithubCopilotRefreshToken(winrt::hstring refreshToken)
    {
        _Settings.GlobalSettings().AIInfo().GithubCopilotRefreshToken(refreshToken);
        _NotifyChanges(L"AreGithubCopilotTokensSet");
    }

    bool AISettingsViewModel::GithubCopilotActive()
    {
        return _Settings.GlobalSettings().AIInfo().ActiveProvider() == Model::LLMProvider::GithubCopilot;
    }

    void AISettingsViewModel::GithubCopilotActive(bool active)
    {
        if (active)
        {
            _Settings.GlobalSettings().AIInfo().ActiveProvider(Model::LLMProvider::GithubCopilot);
            _NotifyChanges(L"AzureOpenAIActive", L"OpenAIActive", L"GithubCopilotActive");
        }
    }

    void AISettingsViewModel::InitiateGithubAuth_Click(const IInspectable& /*sender*/, const RoutedEventArgs& /*e*/)
    {
        GithubAuthRequested.raise(nullptr, nullptr);
    }

    void AISettingsViewModel::_OnGithubAuthCompleted()
    {
        // todo: there is a problem here
        //       our copy of _Settings hasn't actually been updated with the new settings, only the actual
        //       _settings over in TerminalPage has the update (we have a copy that doesn't update automatically)
        //       so we think the tokens are still empty
        //       we could potentially solve this by checking the vault directly but that feels icky
        _NotifyChanges(L"AreGithubCopilotTokensSet");
    }
}
