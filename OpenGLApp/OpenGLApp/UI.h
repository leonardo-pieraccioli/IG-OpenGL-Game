#pragma once

#include <algorithm>

#include "imgui.h"
#include "MiniEngine/Game.h"
#include "MiniEngine/SoundManager.h"
#include "UpgradeManager.h"

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

enum UISound


{
    ok,
    cancel,
    upgrade,
    heal
};

void playsound(UISound soundID)
{
    switch (soundID)
    {
    case ok:        SoundManager::Instance().playSound("Assets/Sounds/UI/ok.mp3", false); break;
    case cancel:    SoundManager::Instance().playSound("Assets/Sounds/UI/cancel.wav", false); break;
    case upgrade:   SoundManager::Instance().playSound("Assets/Sounds/powerup.wav", false); break;
    case heal:      SoundManager::Instance().playSound("Assets/Sounds/heal.wav", false); break;
    }
}

vector<int> gen3Nums()
{
    vector<int> result;
    random_device rd; // obtain a random number from hardware
    mt19937 eng(rd()); // seed the generator
    uniform_int_distribution<> distr(0, TOT_UPGRADES - 1); // define the range 

    int i = 0;
    while (i < 3) { // loop until you have collected the sufficient number of results
        int randVal = distr(eng);
        if (std::find(std::begin(result), std::end(result), randVal) == std::end(result)) {
            // ^^^^^^^^^^^^ The above part is essential, only add random numbers to the result 
            // which aren't yet contained.
            result.push_back(randVal);
            cout << result[i];
            ++i;
        }
    }
    return result;
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

    // ROUND
    // -----
    {
		ImGui::PushFont(Game::Instance().font_SA_medium);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPosY(10);
		std::string roundText = "Round: " + std::to_string(Game::Instance().getRound());
		ImGui::Text(roundText.c_str());
		ImGui::PopFont();
    }

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
        playsound(ok);
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Restart", { buttonWidth, 0.f }))
    {
        playsound(ok);
        Game::Instance().resetGame();
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Main menu", { buttonWidth, 0.f }))
    {
        playsound(ok);
        Game::Instance().resetGame();
        Game::Instance().ChangeGameState(GameState::Menu);
    }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Quit", { buttonWidth, 0.f }))
    {
        playsound(cancel);
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
        playsound(ok);
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Controls", { buttonWidth, 0.f }))
    {
        playsound(ok);
        Game::Instance().ChangeGameState(GameState::Controls);
    }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Quit", { buttonWidth, 0.f }))
    {
        playsound(cancel);
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
        playsound(ok);
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
        playsound(ok);
        Game::Instance().resetGame();
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
    if (ImGui::Button("Quit", { buttonWidth, 0.f }))
    {
        playsound(cancel);
        Game::Instance().ChangeGameState(GameState::Quit);
    }

    ImGui::PopFont();
    ImGui::End();
}

#pragma endregion

#pragma region Shop

void UIShop()
{
    auto& ships = Game::Instance().player->shipArray;

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

    int healingCostFactor = 5 + Game::Instance().getRound() * 0.5 - 0.5;

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
                playsound(heal);
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

    // RANDOM UPGRADE SELECTION
    // ------------------------
    static bool choiceMade = false;
    static std::vector<int> randUpgIdx = gen3Nums();
	static bool clicked[3] = { false, false, false };
    

    ImGui::SeparatorText("Upgrades");
    ImVec2 upgradeButton = { 500, 50 };

    float upgOffset = 1;
    for (int i = 0; i < 3; i++)
    {
        ImGui::PushID(i);
        UpgradeIndex upgradeIdx = static_cast<UpgradeIndex>(randUpgIdx[i]);
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() * (upgOffset/6) - upgradeButton.x / 2);
        const char * upgradeName = UpgradeManager::Instance().getUpgradeName(upgradeIdx);
        int upgradeCost = UpgradeManager::Instance().getUpgradeCost(upgradeIdx);
        bool hasReachedMax = UpgradeManager::Instance().hasReachedMax(upgradeIdx);
        std::string buttonText = std::string(upgradeName) + ": " + std::to_string(upgradeCost);

		bool isClicked = clicked[i];

        if ( hasReachedMax )
        {
            buttonText += " MAX";
            ImGui::BeginDisabled();
        }
		else if (isClicked)
		{
			ImGui::BeginDisabled();
		}

        if (ImGui::Button(buttonText.c_str(), upgradeButton))
        {
            if (Game::Instance().player->getMoney() >= upgradeCost)
            {
				// clicked[i] = true;
                playsound(upgrade);
                Game::Instance().player->addMoney(-upgradeCost);
                UpgradeManager::Instance().makeUpgrade(upgradeIdx);
                Game::Instance().upgrade(upgradeIdx);
            }
            else
            {
                playsound(cancel);
            }
        }
        if (hasReachedMax || isClicked)
            ImGui::EndDisabled();

        ImGui::PopID();
        ImGui::SameLine();
		upgOffset += 2;
    }

    ImGui::NewLine();
    ImGui::PopFont();

    ImGui::PushFont(Game::Instance().font_SA_small);

    ImGui::SeparatorText("Current stats");
    ImGui::NewLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 500);
    for (int i = 0; i < TOT_UPGRADES; i++)
    {
        UpgradeIndex upgIdx = static_cast<UpgradeIndex>(i);
        const char* upgradeName = UpgradeManager::Instance().getUpgradeName(upgIdx);
        float upgradeValue = UpgradeManager::Instance().getGenericCurrentValue(upgIdx);
		std::string upgradeValueStr = std::to_string(upgradeValue);
		upgradeValueStr = upgradeValueStr.substr(0, upgradeValueStr.find(".") + 3);
        std::string buttonText = std::string(upgradeName) + " : " + upgradeValueStr + "\t";
        ImGui::Text(buttonText.c_str());
        ImGui::SameLine();
        if (i % 2 == 1)
        {
            ImGui::NewLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 500);
        }
    }

    ImGui::PopFont();
    
    // HEAL PLANET
    // -----------
    ImGui::PushFont(Game::Instance().font_SA_small);
    ImVec2 progressBarDim = { 500, 40 };
    int healPlanetCost = 200 + Game::Instance().getRound() - 1;
    ImGui::SetCursorPos({ ImGui::GetWindowWidth() / 2 - progressBarDim.x / 2, ImGui::GetWindowHeight() - 75 - progressBarDim.y});
    if (ImGui::Button(std::string("Heal Planet by 15: " + std::to_string(healPlanetCost)).c_str(), progressBarDim) && Game::Instance().player->getMoney() >= 200 && Game::Instance().planet->health.healthStatus() != Game::Instance().planet->health.getMax())
    {
        playsound(heal);
        Game::Instance().player->addMoney(-healPlanetCost);
        Game::Instance().planet->health.Heal(15);
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
        playsound(ok);
        choiceMade = false;
        Game::Instance().ChangeGameState(GameState::Play);
    }

    ImGui::PopFont();
    ImGui::End();
}

#pragma endregion