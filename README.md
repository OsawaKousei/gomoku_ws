# GomokuLib - 五目並べライブラリ

C++で実装された五目並べ（Gomoku）ゲームのためのライブラリです。このライブラリは、五目並べゲームのロジックを提供し、GUI やコンソールなど様々なフロントエンドから利用できます。

## 主な機能

-   任意サイズの盤面作成
-   石の配置と勝敗判定
-   棋譜の記録と再生
-   一手戻す機能
-   ゲーム状態の保存と読み込み

## 構成

このライブラリは以下のクラスで構成されています：

-   `Board` - 五目並べの盤面を管理するクラス
-   `Game` - ゲーム全体の進行を管理するクラス
-   `Stone` - 石の種類を表す列挙型（EMPTY, BLACK, WHITE, DRAW）
-   `MoveResult` - プレイの結果を表す列挙型（SUCCESS, INVALID_MOVE, GAME_OVER）

## 使用方法

### 基本的な使い方

```cpp
#include "GomokuLib/Game.h"
#include <iostream>

int main() {
    // 15x15の盤面でゲームを作成
    GomokuLib::Game game(15);

    // 石を配置（例：中央に黒石）
    int center = 7;
    game.playTurn(center, center);

    // 現在のプレイヤーを取得
    GomokuLib::Stone currentPlayer = game.getCurrentPlayer();

    // 勝敗判定
    if (game.isGameOver()) {
        GomokuLib::Stone winner = game.getWinner();
        // 勝者に応じた処理
    }

    return 0;
}
```

### ゲームの保存と読み込み

```cpp
// ゲーム状態を保存
game.saveGame("game_record.txt");

// 保存したゲームを読み込み
GomokuLib::Game loadedGame = GomokuLib::Game::loadGame("game_record.txt");
```

### 一手戻す

```cpp
bool success = game.undoMove();
if (success) {
    std::cout << "一手戻しました" << std::endl;
} else {
    std::cout << "これ以上戻せません" << std::endl;
}
```

## ビルド方法

このライブラリは、CMake を使用してビルドします。

```bash
mkdir build
cd build
cmake ..
make
```

## ライセンス

このライブラリはオープンソースで提供されており、[MIT ライセンス](LICENSE)の下で配布されています。

## 貢献

バグ報告や機能リクエストは、GitHub の Issue を通じてお知らせください。プルリクエストも歓迎します。
