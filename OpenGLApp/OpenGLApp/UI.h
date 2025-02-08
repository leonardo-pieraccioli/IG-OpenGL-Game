#pragma once

#include "imgui.h"
#include "MiniEngine/Game.h"

// UTILS
// -----
void TextCentered(std::string text, float posY) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::SetCursorPosY(posY);
    ImGui::Text(text.c_str());
}

void AlignForWidth(float width, float alignment = 0.5f)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

#pragma region Play

void UIPlay()
{
    ImGui::SetNextWindowSize({ (float)Game::Instance().SCREEN_WIDTH, (float)Game::Instance().SCREEN_HEIGHT});
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowBgAlpha(0.f);
    ImGui::Begin("HUD", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::PushFont(Game::Instance().font_SA_small);
    ImGui::SetCursorPosX(10);
    ImGui::SetCursorPosY(100);

    for (auto ship : Game::Instance().player->shipArray)
    {
        if (ship->isActive)
        {
            ImGui::Text(std::to_string(ship->health.healthStatus()).c_str());
        }
    }
    ImGui::PopFont();

    ImGui::PushFont(Game::Instance().font_SA_large);

    ImVec2 planetHealthTextDim = ImGui::CalcTextSize(std::to_string(Game::Instance().planet->health.healthStatus()).c_str());
    ImGui::SetCursorPos({ ImGui::GetWindowWidth() / 2 - planetHealthTextDim.x / 2, ImGui::GetWindowHeight() / 2 - planetHealthTextDim.y / 2 });
    ImGui::TextColored({ 0, 1, 0, 1 }, std::to_string(Game::Instance().planet->health.healthStatus()).c_str());
    ImGui::PopFont();

    ImGui::PushFont(Game::Instance().font_SA_medium);
    std::string roundTimeText = TimerManager::GetTimer("Round Timer")->getHH_MM_SS_MS();
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize("00:00:0,000").x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::SetCursorPosY(10);
    ImGui::Text(roundTimeText.c_str());
    ImGui::PopFont();

    ImGui::PushFont(Game::Instance().font_SA_medium);
    ImGui::SetCursorPos({ 10, (float)Game::Instance().SCREEN_HEIGHT - 70 });

    std::string scoreText = "Score: " + std::to_string(Game::Instance().player->getScore());
    ImGui::Text(scoreText.c_str());

    ImGui::PopFont();


    ImGui::End();

}

#pragma endregion

#pragma region Pause

void UIPause()
{
    ImGui::SetNextWindowSize({ (float) Game::Instance().SCREEN_WIDTH, (float)Game::Instance().SCREEN_HEIGHT});
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowBgAlpha(0.50f);
    ImGui::Begin("Pause Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::PushFont(Game::Instance().font_SA_large);

    TextCentered("Pause", ImGui::GetWindowHeight() / 3);

    float buttonWidth = ImGui::CalcTextSize("Pause").x;
    ImGui::PopFont();
    ImGui::PushFont(Game::Instance().font_SA_medium);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Resume", { buttonWidth, 0.f }))
    {
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Restart", { buttonWidth, 0.f }))
    {
        Game::Instance().resetGame();
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Quit", { buttonWidth, 0.f }))
    {
        Game::Instance().ChangeGameState(GameState::Quit);
    }

    ImGui::PopFont();
    ImGui::End();
}

#pragma endregion

#pragma region Menu

void UIMenu()
{
    ImGui::SetNextWindowSize({ (float)Game::Instance().SCREEN_WIDTH, (float)Game::Instance().SCREEN_HEIGHT });
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowBgAlpha(0.15f);
    ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::PushFont(Game::Instance().font_SA_menu);

    TextCentered("Space", ImGui::GetWindowHeight() / 4);
    TextCentered("Defender", ImGui::GetWindowHeight() / 4 + 60);

    float buttonWidth = ImGui::CalcTextSize("Defender").x;
    ImGui::PopFont();
    ImGui::PushFont(Game::Instance().font_SA_large);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Play", { buttonWidth, 0.f }))
    {
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Quit", { buttonWidth, 0.f }))
    {
        Game::Instance().ChangeGameState(GameState::Quit);
    }

    ImGui::PopFont();
    ImGui::End();
}

#pragma endregion

#pragma region GameOver

void UIGameOver()
{
    ImGui::SetNextWindowSize({ (float)Game::Instance().SCREEN_WIDTH, (float)Game::Instance().SCREEN_HEIGHT });
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowBgAlpha(.5f);
    ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::PushFont(Game::Instance().font_SA_large);

    TextCentered("Game Over!", ImGui::GetWindowHeight() / 3);

    float buttonWidth = ImGui::CalcTextSize("game over").x;
    ImGui::PopFont();
    ImGui::PushFont(Game::Instance().font_SA_medium);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Retry", { buttonWidth, 0.f }))
    {
        Game::Instance().resetGame();
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Quit", { buttonWidth, 0.f }))
    {
        Game::Instance().ChangeGameState(GameState::Quit);
    }

    ImGui::PopFont();
    ImGui::End();
}

#pragma endregion

#pragma region Shop

void UIShop()
{
    auto &ships = Game::Instance().player->shipArray;

    ImGui::SetNextWindowSize({ (float)Game::Instance().SCREEN_WIDTH, (float)Game::Instance().SCREEN_HEIGHT });
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowBgAlpha(0.50f);
    ImGui::Begin("Shop", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::PushFont(Game::Instance().font_SA_large);
    TextCentered("Shop", 100);
    ImGui::PopFont();

    ImGui::PushFont(Game::Instance().font_SA_small);

    // ------------
    // Ship healing

    int healingCostFactor = 5;

    ImVec2 buttonDim = { ((ImGui::GetWindowWidth() - 20) / 4) - 5, 50 };
    ImGui::SeparatorText("Ship Health");
    ImGui::SetCursorPosX(10);

    if (ImGui::BeginTable("table1", 4))
    {
        ImGui::TableNextRow();
        for (int i = 0; i < ships.size(); i++)
        {
            ImGui::TableSetColumnIndex(i);
            float progress = ships[i]->isActive ? (float)ships[i]->health.healthStatus() / (float)ships[i]->health.getMax() : 0.f;
            ImGui::ProgressBar(progress, buttonDim);
        }
        ImGui::TableNextRow();

        for (int i = 0; i < ships.size(); i++)
        {
            ImGui::TableSetColumnIndex(i);
            std::string buttonText = "Heal Ship " + std::to_string(i + 1);
            int healCost = (ships[i]->health.getMax() - ships[i]->health.healthStatus()) * healingCostFactor;
            if (!ships[i]->isActive)
                ImGui::BeginDisabled();
            if (ImGui::Button(buttonText.c_str(), buttonDim) && Game::Instance().player->getMoney() >= healCost)
            {
                Game::Instance().player->addMoney(-healCost);
                ships[i]->health.Heal(100);
            }
            if (!ships[i]->isActive)
                ImGui::EndDisabled();
        }
        ImGui::EndTable();
    }

    ImGui::SeparatorText("Upgrades");
    ImVec2 upgradeButton = { 300, 100 };
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 4 - upgradeButton.x / 2);
    if (ImGui::Button("Add Ship\n500", upgradeButton) && Game::Instance().player->getMoney() >= 500)
    {
        for (int i = 0; i < ships.size(); i++)
        {
            if (!ships[i]->isActive)
            {
                Game::Instance().player->addMoney(-500);
                ships[i]->isActive = true;
                break;
            }
        }
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - upgradeButton.x / 2);
    if (ImGui::Button("Heal Planet\n200", upgradeButton) && Game::Instance().player->getMoney() >= 200)
    {
        Game::Instance().player->addMoney(-200);
        Game::Instance().planet->health.Heal(50);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(3 * ImGui::GetWindowWidth() / 4 - upgradeButton.x / 2);
    if (ImGui::Button("Placeholder\n1500", upgradeButton) && Game::Instance().player->getMoney() >= 0)
    {

    }

    TextCentered("Planet Health: " + std::to_string(Game::Instance().planet->health.healthStatus()), ImGui::GetWindowHeight() / 2 + 100);

    ImGui::SetCursorPos({ 1.5f * buttonDim.x / 2, ImGui::GetWindowHeight() - 100 + buttonDim.y / 2 });
    string moneyText = "Money: " + std::to_string(Game::Instance().player->getMoney());
    ImGui::Text(moneyText.c_str());

    ImGui::SetCursorPos({ ImGui::GetWindowWidth() - 3 * buttonDim.x / 2, ImGui::GetWindowHeight() - 100 });
    if (ImGui::Button("Continue", { ImGui::CalcTextSize(" Continue ").x , buttonDim.y }))
    {
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::PopFont();
    ImGui::End();
}

#pragma endregion