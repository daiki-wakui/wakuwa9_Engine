#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include <memory>

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    //次のシーンを生成
   // BaseScene* newScene = nullptr;
    std::unique_ptr<BaseScene> newScene = nullptr;
        //std::make_unique<PostEffect>();

    if (sceneName == "TITLE") {
        newScene = std::make_unique<TitleScene>();
    }
    else if (sceneName == "GAME") {
        newScene = std::make_unique<GameScene>();
    }

    return newScene;
}
