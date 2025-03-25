#pragma once

namespace GomokuLib
{

    // 石の種類を表す列挙型
    enum class Stone
    {
        EMPTY, // 空
        BLACK, // 黒
        WHITE, // 白
        DRAW   // 引き分け（checkWinnerの返り値としてのみ使用）
    };

    // プレイの結果を表す列挙型
    enum class MoveResult
    {
        SUCCESS,      // 成功
        INVALID_MOVE, // 無効な動き
        GAME_OVER     // ゲーム終了
    };

} // namespace GomokuLib
