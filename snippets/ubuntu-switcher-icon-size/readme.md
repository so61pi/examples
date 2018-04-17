# 1. Prepare
```shell
sudo apt-get update
sudo apt-get build-dep unity
```


# 2. Get unity source code
```shell
mkdir /tmp/unity-src
cd /tmp/unity-src
apt-get source unity
```


# 3. Modify SwitcherViewer.cpp & test_switcher_view.cpp
```cpp
//
// ./launcher/SwitcherView.cpp
//
namespace
{
  RawPixel const VERTICAL_PADDING = 45_em;
  RawPixel const BORDER_SIZE = 25_em;      // 50
  RawPixel const FLAT_SPACING = 10_em;     // 20
  RawPixel const ICON_SIZE = 64_em;        // 128
  RawPixel const MINIMUM_SPACING = 10_em;
  RawPixel const TILE_SIZE = 75_em;        // 150
  RawPixel const SPREAD_OFFSET = 100_em;
  RawPixel const EXTRA_ICON_SPACE = 10_em;
  RawPixel const TEXT_SIZE = 15_em;

  unsigned int const ANIMATION_LENGTH = 250;
  unsigned int const MAX_DIRECTIONS_CHANGED = 3;
  unsigned int const SCROLL_WHEEL_EVENTS_DISTANCE = 75;
  double const TEXT_TILE_MULTIPLIER = 3.5;
}
```

```cpp
//
// ./tests/test_switcher_view.cpp
//
TEST_F(TestSwitcherView, Initiate)
{
  const int VERTICAL_PADDING = 45;
  EXPECT_FALSE(switcher.render_boxes);
  EXPECT_EQ(switcher.border_size, 25);      // 50
  EXPECT_EQ(switcher.flat_spacing, 10);     // 20
  EXPECT_EQ(switcher.icon_size, 64);        // 128
  EXPECT_EQ(switcher.minimum_spacing, 10);
  EXPECT_EQ(switcher.tile_size, 75);        // 150
  EXPECT_EQ(switcher.vertical_size, switcher.tile_size + VERTICAL_PADDING * 2);
  EXPECT_EQ(switcher.text_size, 15);
  EXPECT_EQ(switcher.animation_length, 250);
  EXPECT_EQ(switcher.monitor, 0);
  ASSERT_NE(switcher.text_view_, nullptr);
  ASSERT_NE(switcher.icon_renderer_, nullptr);
  EXPECT_EQ(switcher.icon_renderer_->pip_style, ui::OVER_TILE);
  EXPECT_DOUBLE_EQ(switcher.GetCurrentProgress(), 0.0f);
}
```


# 4. Build deb files
```shell
dpkg-buildpackage -rfakeroot -us -uc -b
```


# 5. Install new unity
```shell
sudo dpkg -i ../unity_*.deb
```

  - You don't need to install other deb files.


# 6. Note
  - If you have problem with unmet dependencies (`xorg-video-abi-15`, `xserver-xorg-core`) , run `sudo apt-get install xserver-xorg-dev`.
  - If your indicators are missing, run `killall unity-panel-service`.
