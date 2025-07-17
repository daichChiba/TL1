#include "GameScene.h"
#include "KamataEngine.h"

#include <cassert>  //assert
#include <fstream>  //ifstream
#include <json.hpp> //nlohman::
#include <string>   //string
#include <vector>   //vector

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	levelData = new LevelData();

	//-------------------------------------------------------------------
	// jsonファイルのデシリアライズ化
	//-------------------------------------------------------------------

	// jsonファイルのパス名
	const std::string fullpath = std::string("Resources/levels/") + std::string("test.json");

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	nlohmann::json deserialized; // deserialized : 逆シリアライズ化
	                             // ⇒1つの文字列をメモリ中のデータ構造化すること
	                             // serialize:一列に並べる操作の事
	                             // ⇒1つの文字列で表現する「直列化」のこと

	// ファイルから読み込み、メモリへ格納
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());         // object か ※json形式には様々な型がある
	                                          // object型はその中でも「キーと値のペアを持つ構造」つまり連想配列が扱えるか聞いている
	assert(deserialized.contains("name"));    //"name"が含まれているか
	assert(deserialized["name"].is_string()); //["name"]は文字列か？

	// "name"を文字列として取得
	levelData->name = deserialized["name"].get<std::string>();
	assert(levelData->name == "scene"); // それは"scene"か？

	//// "name"を文字列として取得
	// std::string name = deserialized["name"].get<std::string>();
	//// 正しいレベルデータかチェック
	// assert(name.compare("scene") == 0);

	//// レベルデータ格納用インスタンスを生成
	// LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		// オブジェクト 一つ分の妥当性のチェック
		assert(object.contains("type")); //"type""が含まれているか
		// std::string type = object["type"].get<std::string>();

		if (object["type"].get<std::string>() == "MESH") {
			// 1個分の要素の準備
			levelData->objects.emplace_back(ObjectData{});
			ObjectData& objectData = levelData->objects.back(); // 追加要素の参照を用意し可読性もよくなる

			objectData.type = object["type"].get<std::string>(); //"type"
			objectData.name = object["name"].get<std::string>(); //"name"

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動 "translation"
			objectData.transform.translation.x = (float)transform["translation"][1];
			objectData.transform.translation.y = (float)transform["translation"][2];
			objectData.transform.translation.z = (float)transform["translation"][0];
			// 回転角 "rotation"
			objectData.transform.rotation.x = -(float)transform["rotation"][1];
			objectData.transform.rotation.y = -(float)transform["rotation"][2];
			objectData.transform.rotation.z = -(float)transform["rotation"][0];
			// 拡大縮小 "scaling"
			objectData.transform.scaling.x = (float)transform["scaling"][1];
			objectData.transform.scaling.y = (float)transform["scaling"][2];
			objectData.transform.scaling.z = (float)transform["scaling"][0];

			//"file_name"
			if (object.contains("file_name")) {
				objectData.file_name = object["file_name"].get<std::string>();
			}

			//-------------------------------------------------------------------
			// レベルデータからオブジェクトを生成、配置
			//-------------------------------------------------------------------
			for (auto& objectData_ : levelData->objects) {
				// 登録モデルを検索
				if (models.find(objectData_.file_name) == models.end()) {
					Model* model = Model::CreateFromOBJ(objectData_.file_name);
					models[objectData_.file_name] = model;
				}
				// 3Dオブジェクトを生成
				WorldTransform* newObject = new WorldTransform();
				// 位置の設定 objectData.transform.translation	に入っている
				newObject->translation_ = objectData_.transform.translation;
				// 回転の設定 objectData.transform.rotation		に入っている
				newObject->rotation_ = objectData_.transform.rotation;
				// 拡大縮小　objectData.scaling					に入っている
				newObject->scale_ = objectData_.transform.scaling;

				if (objectData_.file_name == std::string("cube")) {
				}

				newObject->Initialize();

				// 配列に登録
				worldTransforms.push_back(newObject);
			}
		}

		//// MESH
		// if (type.compare("MESH") == 0) {
		//	// 要素追加
		//	levelData->objects.emplace_back(LevelData::ObjectData{});
		//	// 今追加した要素の参照を得る
		//	LevelData::ObjectData& objectData = levelData->objects.back();

		//	if (object.contains("file_name")) {
		//		// ファイル名
		//		objectData.fileName = object["file_name"];
		//	}
		//	// トランスフォームのパラメータ読み込み
		//	nlohmann::json& transform = object["transform"];
		//	// 平行移動 "translation"
		//	objectData.translation.x = (float)transform["translation"][1];
		//	objectData.translation.y = (float)transform["translation"][2];
		//	objectData.translation.z = (float)transform["translation"][0];
		//	// 回転角 "rotation"
		//	objectData.rotation.x = -(float)transform["rotation"][1];
		//	objectData.rotation.y = -(float)transform["rotation"][2];
		//	objectData.rotation.z = -(float)transform["rotation"][0];
		//	// 拡大縮小 "scaling"
		//	objectData.scaling.x = (float)transform["scaling"][1];
		//	objectData.scaling.y = (float)transform["scaling"][2];
		//	objectData.scaling.z = (float)transform["scaling"][0];
		//	// TODO: コライダーのパラメータ読み込み
		//}
		//// 再帰処理
		//// TODO:　オブジェクト走査を再起関数にまとめ、再帰呼出で枝を走査する
		// if (object.contains("children")) {
		// }
	}

	//// レベルデータからオブジェクトを生成、配置
	// for (auto& objectData : levelData->objects) {
	//	// ファイル名から登録済みモデルを検索
	//	Model* model = nullptr;
	//	//decltype(models)
	// }
	camera_.Initialize();
}

void GameScene::Update() {
	for (WorldTransform* worldTransform : worldTransforms) {
		worldTransform->matWorld_ = MathUtility::MakeTranslateMatrix(worldTransform->translation_);
		worldTransform->UpdateMatrix();
	}
	//DrawImgui();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	 int i = 0;

	//-------------------------------------------------------------------//
	// レベルデータからオブジェクトを生成、配置
	//-------------------------------------------------------------------//
	//for (auto& objectData : levelData->objects) {
	//	// モデルファイル名
	//	Model* model = nullptr;
	//	decltype(models)::iterator it = models.find(objectData.file_name);
	//	if (it != models.end()) {
	//		model = it->second;
	//	}
	//	model->Draw(*worldTransforms[i], camera_);
	//	i++;
	//}

	// int i = 0;
	//-------------------------------------------------------------------
	// レベルデータからオブジェクトを生成、配置
	//-------------------------------------------------------------------
	 for (auto& objectData : levelData->objects) {
		models[objectData.file_name]->Draw(*worldTransforms[i], camera_);
		i++;
	 }

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::DrawImgui() {
	ImGui::Begin("Game");
	for (int i = 0; i < worldTransforms.size(); i++) {
		ImGui::DragFloat3("model", &worldTransforms[i]->translation_.x, 0.1f);
	}
	ImGui::End();
}
