#pragma once
#include "KamataEngine.h"
#include <map>

// ゲームシーン
class GameScene {
	//--------------------------------------
	// レベルデータを格納するための構造体
	//--------------------------------------
	struct ObjectData {
		std::string type; // "type"
		std::string name; // "name"

		struct Transform {
			KamataEngine::Vector3 translation; // translation
			KamataEngine::Vector3 rotation;    // rotation
			KamataEngine::Vector3 scaling;     // scaling
		};

		Transform transform; // メンバの準備

		//"file_name"
		std::string file_name;

		//// オブジェクトのコンテナ
		// std::vector<ObjectData> objects;
	};

	// レベルデータ
	struct LevelData {
		// "name"
		std::string name;
		// "objects"
		std::vector<ObjectData> objects;
	};

public:
	// コンストラクタ
	GameScene();
	// デストラクタ
	~GameScene();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();


private:
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;


	// モデルデータコンテナ
	std::map<std::string, KamataEngine::Model*> models;

	// ワールドトランスフォーム
	std::vector<KamataEngine::WorldTransform*> worldTransforms;

	//-------------------------------------------------------------------
	// レベルデータを構造体に格納していく
	//-------------------------------------------------------------------
	LevelData* levelData = nullptr;


	KamataEngine::Camera camera_;

};
