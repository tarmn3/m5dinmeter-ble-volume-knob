#include <M5Unified.h>
#include <BleKeyboard.h>
#include <ESP32Encoder.h>
#include <OneButton.h>

// iPhone対応 NimBLEモード
BleKeyboard bleKeyboard("M5DinMeter-Vol", "M5Stack", 100);

// ピン定義
constexpr int PIN_ENCODER_A   = 41;
constexpr int PIN_ENCODER_B   = 40;
constexpr int PIN_ENCODER_BTN = 42;

ESP32Encoder encoder;
OneButton btn(PIN_ENCODER_BTN, true);

// int volume = 50;  // 未使用のため削除（使うなら論理ボリュームとして利用する）
int64_t lastEncoderCount = 0;
bool wasConnected = false;

// 接続状態に応じた画面表示
void updateConnectionDisplay(bool isConnected) {
    if (isConnected) {
        M5.Display.fillScreen(TFT_BLUE);
        M5.Display.setTextColor(WHITE, TFT_BLUE);
        M5.Display.setCursor(10, 20);
        M5.Display.print("CONNECTED!");
    } else {
        M5.Display.fillScreen(BLACK);
        M5.Display.setTextColor(WHITE, BLACK);
        M5.Display.setCursor(10, 20);
        M5.Display.print("Waiting...");
    }
}

// ボタンクリック処理
void handleClick() {
    M5.Display.fillScreen(TFT_RED);
    if (bleKeyboard.isConnected()) {
        bleKeyboard.write(KEY_MEDIA_MUTE);
    }
    delay(100);
    // 接続状態に応じた画面表示を復元
    updateConnectionDisplay(bleKeyboard.isConnected());
}

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    
    Serial.begin(115200);
    Serial.println("Starting NimBLE...");

    // 画面初期化
    M5.Display.setRotation(1);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.fillScreen(BLACK);
    M5.Display.drawString("iPhone Pair", 10, 20);

    // エンコーダ設定
    ESP32Encoder::useInternalWeakPullResistors = (puType)1; // enum 定数名があればそれを使うとより安全
    encoder.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
    encoder.setCount(50 * 2);  // 50ステップ相当に初期化

    btn.attachClick(handleClick);

    bleKeyboard.begin();
}

void loop() {
    M5.update();
    btn.tick();

    bool isConnected = bleKeyboard.isConnected();
    if (isConnected != wasConnected) {
        updateConnectionDisplay(isConnected);
        Serial.println(isConnected ? "BLE Connected" : "BLE Disconnected");
        wasConnected = isConnected;
    }

    int64_t currentCount = encoder.getCount() / 2;
    int64_t diff = currentCount - lastEncoderCount;

    if (diff != 0) {
        if (isConnected) {
            M5.Display.setCursor(10, 50);
            if (diff > 0) {
                M5.Display.print("UP        ");
                // エンコーダの差分ステップ数分だけボリュームアップ
                for (int64_t i = 0; i < diff; ++i) {
                    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
                }
            } else {
                M5.Display.print("DOWN      ");
                // エンコーダの差分ステップ数分だけボリュームダウン
                for (int64_t i = 0; i < -diff; ++i) {
                    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
                }
            }
        }
        lastEncoderCount = currentCount;
    }

    delay(5);
}
