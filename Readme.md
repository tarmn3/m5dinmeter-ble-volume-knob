# M5DinMeter BLE Volume Knob

M5Stack 社の **M5DinMeter**（実体は M5StampS3 ベース）を使って、  
PC やスマートフォンの音量を操作する **BLE キーボード互換のボリュームノブ**です。

- ハードウェア: M5DinMeter (M5StampS3)
- フレームワーク: PlatformIO + Arduino
- 機能:
  - ロータリーエンコーダで音量アップ / ダウン
  - エンコーダの差分に応じて複数ステップ分を一気に送信
  - プッシュボタンでミュートトグル（メディアキー）
  - 接続状態を本体ディスプレイに表示（`CONNECTED!` / `Waiting...`）

English summary is below.

---

## Hardware

- M5Stack M5DinMeter (M5StampS3)
- On-board rotary encoder (A/B + push button)
- On-board display

ピンアサインはソースコード内の `PIN_ENCODER_A`, `PIN_ENCODER_B`, `PIN_ENCODER_BTN` を参照してください。

```cpp
constexpr int PIN_ENCODER_A   = 41;
constexpr int PIN_ENCODER_B   = 40;
constexpr int PIN_ENCODER_BTN = 42;
