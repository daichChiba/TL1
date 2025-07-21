#include "Scene/GameScene.h"
#include <Windows.h>
using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"LE3D_16_チバ_ダイチ_TL1");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//// ImGuiの初期化
	//ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	//imguiManager->Initialize();

	// ゲームシーンの生成
	GameScene* gameScene = new GameScene();
	// ゲームシーンの初期化
	gameScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (Update()) {
			break;
		}

		//// ImGui受付開始
		//imguiManager->Begin();

		// ゲームシーンの更新
		gameScene->Update();

		//// ImGui受付終了
		//imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		// ここに描画処理を記述する

		// ゲームシーンの描画
		gameScene->Draw();

		//// ImGui描画
		//imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	delete gameScene;
	// ゲームシーンの解放
	gameScene = nullptr;

	//// ImGui解放
	//imguiManager->Finalize();

	// エンジンの終了処理
	Finalize();

	return 0;
}
