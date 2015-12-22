#include "AnimationRes.h"
#include "GTAnimation.h"
#include "GameTool.h"

#include "GameData.h"
#include "JsonWrapper.h"

namespace TrinGame {
  class Animation {
  public:
    std::vector<std::string> frames;
    float duration_per_frame;

    Animation () {}
    Animation (const Animation &animation) {
      frames = animation.frames;
      duration_per_frame = animation.duration_per_frame;
    }

    void handleJsonValue(ValueWrapper& value) {
      duration_per_frame = value.getDouble("duration_per_frame");
      value.getVector("frames", frames);
    }
  };

  class AnimationSet {
  public:
    AnimationSet () {}
    AnimationSet (const AnimationSet &set) {
      animations = set.animations;
      name = set.name;
    }

    void load (bool isLoad) {
      if (!isLoad) {
        GTAnimation::unloadAnimationSet(name.c_str());
        return;
      }

      GTAnimation *ani = GTAnimation::startAnimationSet(animations.size());
      for (std::vector<Animation>::iterator iterator = animations.begin(); iterator != animations.end(); iterator++) {
        Animation &item = *iterator;
        addAnimation(ani, item);
      }
      ani->endAnimationSet();
      GTAnimation::loadAnimationSet(ani, name.c_str());
    }

    void handleJsonValue(ValueWrapper& value) {
      name = value.getString("name");
      value.getVector("animations", animations);
    }

  private:
    std::vector<Animation> animations;
    std::string name;

    void addAnimation(GTAnimation* ani, Animation& animation) {
      int count = animation.frames.size();
      ani->startAnimation(count, animation.duration_per_frame*count);
      for (std::vector<std::string>::iterator iterator = animation.frames.begin(); iterator != animation.frames.end(); iterator++) {
        ani->addFrame(iterator->c_str());
      }
      ani->endAnimation();
    }
  };
}

std::vector<TrinGame::AnimationSet> gAnimationSets;

namespace AnimationSets {
void loadAnimationSets () {
  loadVectorFromJsonFile("data/animation_sets.json", gAnimationSets);
}
}

// TODO:refactor the magic numbers
void assetMainRole(const char* param, bool isload) { gAnimationSets[0].load(isload); }
void assetMainGirl(const char* param, bool isload) { gAnimationSets[1].load(isload); }
void assetMainMusashi(const char* param, bool isload) { gAnimationSets[2].load(isload); }
void  assetMainPussy(const char* param, bool isload) { gAnimationSets[3].load(isload); }
void assetEnemy(const char* param, bool isload) { gAnimationSets[4].load(isload); }
void assetMninja(const char* param, bool isload) { gAnimationSets[5].load(isload);  }
void assetHninja(const char* param, bool isload) { gAnimationSets[6].load(isload);  }
void assetSamurai(const char* param, bool isload) { gAnimationSets[7].load(isload);  }
void assetArcher(const char *param, bool isload) { gAnimationSets[8].load(isload);  }
void assetMerchant(const char* param, bool isload) { gAnimationSets[9].load(isload);  }
void assetMechanic(const char* param, bool isload) { gAnimationSets[10].load(isload);  }
void assetPumpkin(const char* param, bool isload) { gAnimationSets[11].load(isload);  }
void assetSanta(const char* param, bool isload) { gAnimationSets[12].load(isload);  }
void assetBlade(const char* param, bool isload) { gAnimationSets[13].load(isload);  }
void assetEffect(const char *param, bool isload) { gAnimationSets[14].load(isload);  }
void assetMsg(const char* param, bool isload) { gAnimationSets[15].load(isload);  }
void assetItem(const char *param, bool isload) { gAnimationSets[16].load(isload);  }
void assetBullets(const char* param, bool isload) { gAnimationSets[17].load(isload);  }
void assetMisc(const char *param, bool isload) { gAnimationSets[18].load(isload);  }
void assetUI(const char *param, bool isload) { gAnimationSets[19].load(isload);  }
