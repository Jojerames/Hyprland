#include "../shared.hpp"
#include "../../shared.hpp"
#include "../../hyprctlCompat.hpp"
#include "tests.hpp"
#include <cstring>
#include <string>

static int ret = 0;

struct SWindowSize {
    int x;
    int y;
};

struct SResizeDirection {
    int A;
    int B;
    int C;
    int D;
};

static SResizeDirection getResizeDirection(int cornerMode, bool horizontal) {

    struct SResizeDirection resizeDir;

    resizeDir.A = 1;
    resizeDir.D = -1;

    if (horizontal) {
        if (cornerMode == 0) {
            resizeDir.B = -1;
            resizeDir.C = 1;
        } else if (cornerMode == 2 || cornerMode == 3) {
            resizeDir.B = 1;
            resizeDir.C = 1;
        } else {
            resizeDir.B = -1;
            resizeDir.C = -1;
        }
        return resizeDir;
    }

    // Vertical directions

    if (cornerMode == 0) {
        resizeDir.B = -1;
        resizeDir.C = 1;
    } else if (cornerMode == 3 || cornerMode == 4) {
        resizeDir.B = 1;
        resizeDir.C = 1;
    } else {
        resizeDir.B = -1;
        resizeDir.C = -1;
    }
    return resizeDir;
}

static std::string getWindowAttribute(const std::string& winInfo, const std::string& attr) {
    auto pos = winInfo.find(attr);
    if (pos == std::string::npos) {
        NLog::log("{}Wrong window attribute", Colors::RED);
        ret = 1;
        return "Wrong window attribute";
    }
    auto pos2 = winInfo.find('\n', pos);
    return winInfo.substr(pos, pos2 - pos);
}

static SWindowSize getActiveWindowSize() {
    SWindowSize       windowSize;

    const std::string SIZE = getWindowAttribute(getFromSocket("/activewindow"), "size:");
    {
        size_t      space = SIZE.find(' ');
        size_t      comma = SIZE.find(',');
        std::string xs    = SIZE.substr(space + 1, comma - space - 1);
        std::string ys    = SIZE.substr(comma + 1);
        // std::string xs = SIZE.substr(5, 8);
        // std::string ys = SIZE.substr(5, 8);
        windowSize.x = stoi(xs);
        windowSize.y = stoi(ys);
    }

    return windowSize;
}

static bool horizontalResizeTest(int cornerMode) {

    const struct SResizeDirection RESIZE_DIR = getResizeDirection(cornerMode, true);

    // Spawn Kitties (meow :3) horizontal (A|B|C|D)
    Tests::spawnKitty("KittyA");

    getFromSocket("/dispatch layoutmsg preselect right");
    Tests::spawnKitty("KittyC");

    getFromSocket("/dispatch focuswindow class:KittyA");
    getFromSocket("/dispatch layoutmsg preselect right");
    Tests::spawnKitty("KittyB");
    struct SWindowSize kittyBSize = getActiveWindowSize();
    getFromSocket("/dispatch focuswindow class:KittyA");
    struct SWindowSize kittyASize = getActiveWindowSize();

    getFromSocket("/dispatch focuswindow class:KittyC");
    getFromSocket("/dispatch layoutmsg preselect right");
    Tests::spawnKitty("KittyD");
    struct SWindowSize kittyDSize = getActiveWindowSize();
    getFromSocket("/dispatch focuswindow class:KittyC");
    struct SWindowSize kittyCSize = getActiveWindowSize();

    // Kitty resize tests!!!

    {
        // Kitty A resize test; 451,1036
        getFromSocket("/dispatch focuswindow class:KittyA");
        getFromSocket("/dispatch resizeactive 10 0");
        SWindowSize newSize = getActiveWindowSize();
        EXPECT((newSize.x == kittyASize.x + (RESIZE_DIR.A * 10)), true)
        getFromSocket("/dispatch resizeactive -10 0");

        // End Kitty A resize test
    }
    {
        // Kitty B resize test; 466,1036
        getFromSocket("/dispatch focuswindow class:KittyB");
        getFromSocket("/dispatch resizeactive 10 0");
        SWindowSize newSize = getActiveWindowSize();
        EXPECT((newSize.x == kittyBSize.x + (RESIZE_DIR.B * 10)), true)
        getFromSocket("/dispatch resizeactive -10 0");

        // End Kitty B resize test
    }
    {
        // Kitty C resize test; 466,1036
        getFromSocket("/dispatch focuswindow class:KittyC");
        getFromSocket("/dispatch resizeactive 10 0");
        SWindowSize newSize = getActiveWindowSize();
        EXPECT((newSize.x == kittyCSize.x + (RESIZE_DIR.C * 10)), true)
        getFromSocket("/dispatch resizeactive -10 0");

        // End Kitty C resize test
    }
    {
        // Kitty D resize test; 451,1036
        getFromSocket("/dispatch focuswindow class:KittyD");
        getFromSocket("/dispatch resizeactive 10 0");
        SWindowSize newSize = getActiveWindowSize();
        EXPECT((newSize.x == kittyDSize.x + (RESIZE_DIR.D * 10)), true)
        getFromSocket("/dispatch resizeactive -10 0");

        // End Kitty D resize test
    }

    Tests::killAllWindows();

    return !ret;
}

static bool verticalResizeTest(int cornerMode) {

    const struct SResizeDirection RESIZE_DIR = getResizeDirection(cornerMode, false);

    // Spawn Kitties (meow :3) horizontal (A|B|C|D)
    Tests::spawnKitty("KittyA");

    getFromSocket("/dispatch layoutmsg preselect bottom");
    Tests::spawnKitty("KittyC");

    getFromSocket("/dispatch focuswindow class:KittyA");
    getFromSocket("/dispatch layoutmsg preselect bottom");
    Tests::spawnKitty("KittyB");
    struct SWindowSize kittyBSize = getActiveWindowSize();
    getFromSocket("/dispatch focuswindow class:KittyA");
    struct SWindowSize kittyASize = getActiveWindowSize();

    getFromSocket("/dispatch focuswindow class:KittyC");
    getFromSocket("/dispatch layoutmsg preselect bottom");
    Tests::spawnKitty("KittyD");
    struct SWindowSize kittyDSize = getActiveWindowSize();
    getFromSocket("/dispatch focuswindow class:KittyC");
    struct SWindowSize kittyCSize = getActiveWindowSize();

    // Kitty resize tests!!!

    {
        // Kitty A resize test; 451,1036
        getFromSocket("/dispatch focuswindow class:KittyA");
        getFromSocket("/dispatch resizeactive 0 10");
        SWindowSize newSize = getActiveWindowSize();
        EXPECT((newSize.y == kittyASize.y + (RESIZE_DIR.A * 10)), true)
        getFromSocket("/dispatch resizeactive 0 -10");

        // End Kitty A resize test
    }
    {
        // Kitty B resize test; 466,1036
        getFromSocket("/dispatch focuswindow class:KittyB");
        getFromSocket("/dispatch resizeactive 0 10");
        SWindowSize newSize = getActiveWindowSize();
        EXPECT((newSize.y == kittyBSize.y + (RESIZE_DIR.B * 10)), true)
        getFromSocket("/dispatch resizeactive 0 -10");

        // End Kitty B resize test
    }
    {
        // Kitty C resize test; 466,1036
        getFromSocket("/dispatch focuswindow class:KittyC");
        getFromSocket("/dispatch resizeactive 0 10");
        SWindowSize newSize = getActiveWindowSize();
        EXPECT((newSize.y == kittyCSize.y + (RESIZE_DIR.C * 10)), true)
        getFromSocket("/dispatch resizeactive 0 -10");

        // End Kitty C resize test
    }
    {
        // Kitty D resize test; 451,1036
        getFromSocket("/dispatch focuswindow class:KittyD");
        getFromSocket("/dispatch resizeactive 0 10");
        SWindowSize newSize = getActiveWindowSize();
        EXPECT((newSize.y == kittyDSize.y + (RESIZE_DIR.D * 10)), true)
        getFromSocket("/dispatch resizeactive 0 -10");

        // End Kitty D resize test
    }

    Tests::killAllWindows();

    return !ret;
}

static bool test() {

    if (getFromSocket("/getoption general:layout").contains("dwindle")) {
        int               resizeCornerTileInt = -1;
        const std::string RESIZE_CORNER_TILE  = getFromSocket("/getoption dwindle:resize_corner_tile");
        if (RESIZE_CORNER_TILE.contains("int: 0"))
            resizeCornerTileInt = 0;
        else if (RESIZE_CORNER_TILE.contains("int: 1"))
            resizeCornerTileInt = 1;
        else if (RESIZE_CORNER_TILE.contains("int: 2"))
            resizeCornerTileInt = 2;
        else if (RESIZE_CORNER_TILE.contains("int: 3"))
            resizeCornerTileInt = 3;
        else if (RESIZE_CORNER_TILE.contains("int: 4"))
            resizeCornerTileInt = 4;
        EXPECT((resizeCornerTileInt == -1), false)
        horizontalResizeTest(resizeCornerTileInt);
        verticalResizeTest(resizeCornerTileInt);
    } else
        ret = 1;

    return !ret;
}

REGISTER_TEST_FN(test)
