#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
#include <random>
#include <cstdlib>
#include <ctime>
#include <stdio.h>

using namespace geode::prelude;


extern FMOD::ChannelGroup* masterGroup;

int randomInt = 1;   // generate a random number in range 85 - 115
float randomNum = static_cast<float>(randomInt) / 100; // divide it to make it 0.85 - 1.15

int updateValue = 0;

std::string valueString;

using namespace geode::prelude;

FMOD::ChannelGroup* masterGroup;

class $modify (PlayLayer) {
    void postUpdate(float p0) {
        PlayLayer::postUpdate(p0);

        bool speedLabel = Mod::get()->getSettingValue<bool>("speed-label-enabled");

        if (Mod::get()->getSettingValue<bool>("enabled") && speedLabel) {

            if (this->getChildByID("sphack-node"_spr)) {
                auto prevLabel = this->getChildByID("sphack-node"_spr);
                prevLabel->removeFromParentAndCleanup(true);
            }

            cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();
            CCLabelBMFont *indicator = CCLabelBMFont::create(valueString.c_str(), "bigFont.fnt");

            indicator->setPosition({ size.width, size.height });
            indicator->setAnchorPoint({ 1.0f, 24.5f });
            indicator->setOpacity(100);
            indicator->setScale(0.4f);

            indicator->setID("sphack-node"_spr);
            this->addChild(indicator);
        }
    }
};

class $modify (CCScheduler)
{
    void update(float dt)
    {
        if (!masterGroup)
        {
            FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);
        }

        updateValue += 1;

        bool modEnabled = Mod::get()->getSettingValue<bool>("enabled");

        if (modEnabled) {

            float timeMax = Mod::get()->getSettingValue<double>("time-max") * 50000;
            float timeMin = Mod::get()->getSettingValue<double>("time-min") * 240;

            if (timeMin > timeMax) {
                timeMin = timeMax;
            }

            std::random_device rand;  
                std::mt19937 gen(rand());  
            std::uniform_int_distribution<>dist(timeMin, timeMax);  
                int timeValue = dist(gen);

            if (timeValue > Mod::get()->getSettingValue<double>("time-max") * 240) {
                timeValue = Mod::get()->getSettingValue<double>("time-max") * 240;
            }

            if (updateValue >= timeValue)
            {
                int sphackMax = Mod::get()->getSettingValue<double>("speedhack-max") * 100;
                int sphackMin = Mod::get()->getSettingValue<double>("speedhack-min") * 100;

                if (sphackMin > sphackMax) {
                    sphackMin = sphackMax;
                }

                std::random_device rand;  
                    std::mt19937 gen(rand());  
                std::uniform_int_distribution<>dis(sphackMin, sphackMax);  
                    int randomInt = dis(gen);
                randomNum = static_cast<float>(randomInt) / 100;
                updateValue = 0;
                valueString = std::to_string(randomNum).substr(0, std::to_string(randomNum).find(".") + 2 + 1) + "x Speed";
            }
            
            masterGroup->setPitch(randomNum);

            CCScheduler::update(dt * randomNum);
            
            return;
                

        } else {
            masterGroup->setPitch(1);
            CCScheduler::update(dt);
            return;
        }
    }
};