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
    ImVec2 ShipProgressBarDim = {300, 30};
    float sh_currentPosY = 50;
    int i = 1;
    for (auto ship : Game::Instance().player->shipArray)
    {
        ImGui::SetCursorPosX( ImGui::GetWindowWidth() - (ShipProgressBarDim.x + ImGui::CalcTextSize("Ship 0").x + 20));
        ImGui::SetCursorPosY( ImGui::GetWindowHeight() - sh_currentPosY);
        if (ship->isActive)
        {
            float shipHealthProgress = (float) ship->health.healthStatus() / (float) ship->health.getMax();
            ImGui::ProgressBar(shipHealthProgress, ShipProgressBarDim);
            ImGui::SameLine();
            std::string shipHealthBarText = "Ship " + std::to_string(i);
            ImGui::Text(shipHealthBarText.c_str());
        }
        sh_currentPosY += 40;
        i++;
    }
    ImGui::PopFont();

    // PLANET HEALTH
    // -------------
    ImGui::PushFont(Game::Instance().font_SA_small);
    ImVec2 PlanetProgressBarDim = {500, 40};
    ImGui::SetCursorPos({ ImGui::GetWindowWidth() / 2 - PlanetProgressBarDim.x / 2, (float)Game::Instance().SCREEN_HEIGHT - 70 });
    float planetHealthProgress = (float) Game::Instance().planet->health.healthStatus() / (float) Game::Instance().planet->health.getMax();
    ImGui::ProgressBar(planetHealthProgress, PlanetProgressBarDim, "Planet Health");
    ImGui::PopFont();

    // TIMER
    // -----
    ImGui::PushFont(Game::Instance().font_SA_medium);
    std::string roundTimeText = TimerManager::GetTimer("Round Timer")->getHH_MM_SS_MS();
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize("00:00:0,000").x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::SetCursorPosY(10);
    ImGui::Text(roundTimeText.c_str());
    ImGui::PopFont();

    // SCORE
    // -----
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
    if (ImGui::Button("Main menu", { buttonWidth, 0.f }))
    {
        Game::Instance().resetGame();
        Game::Instance().ChangeGameState(GameState::Menu);
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
    if (ImGui::Button("Controls", { buttonWidth, 0.f }))
    {
        Game::Instance().ChangeGameState(GameState::Controls);
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

#pragma region Controls

void UIControls()
{
    ImGui::SetNextWindowSize({ (float)Game::Instance().SCREEN_WIDTH, (float)Game::Instance().SCREEN_HEIGHT });
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowBgAlpha(0.15f);
    ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::PushFont(Game::Instance().font_SA_large);

    TextCentered("Controls", ImGui::GetWindowHeight() / 5);

    float buttonWidth = ImGui::CalcTextSize("Pause").x;
    ImGui::PopFont();
    ImGui::PushFont(Game::Instance().font_SA_medium);

    ImGui::NewLine();ImGui::NewLine();
    ImGui::PushFont(Game::Instance().font_SA_small);
    TextCentered("Press A and D to rotate around the planet.", ImGui::GetCursorPosY());
    ImGui::NewLine();
    TextCentered("Press spacebar to shoot enemies.", ImGui::GetCursorPosY());
    ImGui::NewLine();
    TextCentered("When enemies die, they spawn coins.", ImGui::GetCursorPosY());
    TextCentered("Collect the coins to buy upgrades in the shop.", ImGui::GetCursorPosY());
    ImGui::NewLine();
    TextCentered("Be careful! Sometimes coins are dangerous (red) or useful (green).", ImGui::GetCursorPosY());
    TextCentered("Don't click everywhere like a monkey!", ImGui::GetCursorPosY());
    ImGui::PopFont();

    buttonWidth = ImGui::CalcTextSize(" Back to Menu ").x;
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    ImGui::SetCursorPosY(4 * ImGui::GetWindowHeight() / 5);
    if (ImGui::Button("Back to Menu", { buttonWidth, 0.f }))
    {
        Game::Instance().ChangeGameState(GameState::Menu);
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
    ImGui::NewLine();
    ImGui::PopFont();

    ImGui::PushFont(Game::Instance().font_SA_small);

    // ------------
    // Ship healing

    int healingCostFactor = 5;

    ImVec2 buttonDim = { ((ImGui::GetWindowWidth() - 400) / 4), 50 };
    ImGui::SeparatorText("Ship Health");
    ImGui::SetCursorPosX(100);

    if (ImGui::BeginTable("table1", 4))
    {
        ImGui::TableNextRow();
        for (int i = 0; i < ships.size(); i++)
        {
            ImGui::TableSetColumnIndex(i);
            float progress = ships[i]->isActive ? (float)ships[i]->health.healthStatus() / (float)ships[i]->health.getMax() : 0.f;
            std::string barText = "Ship: " + std::to_string(i);
            ImGui::ProgressBar(progress, buttonDim, barText.c_str());
        }
        ImGui::TableNextRow();

        for (int i = 0; i < ships.size(); i++)
        {
            ImGui::TableSetColumnIndex(i);
            int healCost = (ships[i]->health.getMax() - ships[i]->health.healthStatus()) * healingCostFactor;
            std::string buttonText = "Heal: " + std::to_string(healCost);
            if (!ships[i]->isActive || healCost <= 0)
                ImGui::BeginDisabled();
            ImGui::PushID(i);
            if (ImGui::Button(buttonText.c_str(), buttonDim) && Game::Instance().player->getMoney() >= healCost)
            {
                Game::Instance().player->addMoney(-healCost);
                ships[i]->health.Heal(100);
            }
            if (!ships[i]->isActive || healCost <= 0)
                ImGui::EndDisabled();
            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::SeparatorText("Upgrades");
    ImVec2 upgradeButton = { 400, 50 };
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 5 - upgradeButton.x / 2);
    if (ImGui::Button("Add Ship: 500", upgradeButton) && Game::Instance().player->getMoney() >= 500)
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
    if (ImGui::Button("Placeholder: 200", upgradeButton) && Game::Instance().player->getMoney() >= 0)
    {

    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(4 * ImGui::GetWindowWidth() / 5 - upgradeButton.x / 2);
    if (ImGui::Button("Placeholder: 1500", upgradeButton) && Game::Instance().player->getMoney() >= 0)
    {

    }
    ImGui::PopFont();
    
    // HEAL PLANET
    // -----------
    ImGui::PushFont(Game::Instance().font_SA_small);
    ImVec2 progressBarDim = { 500, 40 };
    int healPlanetCost = 200;
    ImGui::SetCursorPos({ ImGui::GetWindowWidth() / 2 - progressBarDim.x / 2, ImGui::GetWindowHeight() - 75 - progressBarDim.y});
    if (ImGui::Button(std::string("Heal Planet by 10: " + std::to_string(healPlanetCost)).c_str(), progressBarDim) && Game::Instance().player->getMoney() >= 200 && Game::Instance().planet->health.healthStatus() != Game::Instance().planet->health.getMax())
    {
        Game::Instance().player->addMoney(-200);
        Game::Instance().planet->health.Heal(10);
    }
    ImGui::PopFont();

    // Money
    ImGui::PushFont(Game::Instance().font_SA_medium);
    string moneyText = "Money: " + std::to_string(Game::Instance().player->getMoney());
    ImGui::SetCursorPos({ ImGui::GetWindowWidth()/4 - ImGui::CalcTextSize(moneyText.c_str()).x, ImGui::GetWindowHeight() - 70});
    ImGui::Text(moneyText.c_str());
    ImGui::PopFont();

    //Planet health
    ImGui::PushFont(Game::Instance().font_SA_small);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - progressBarDim.x / 2);
    float planetHealthProgress = (float)Game::Instance().planet->health.healthStatus() / (float)Game::Instance().planet->health.getMax();
    ImGui::ProgressBar(planetHealthProgress, progressBarDim, "Planet Health");

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth()*3/4);
    if (ImGui::Button("Continue", { ImGui::CalcTextSize(" Continue ").x , progressBarDim.y }))
    {
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::PopFont();
    ImGui::End();
}

#pragma endregion