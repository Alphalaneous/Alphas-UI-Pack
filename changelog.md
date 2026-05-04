# 1.1.1
- Switch to events for mouse buttons and scroll

# 1.1.0
- Add missing Scroll Delta getter and Setter

# 1.0.8
- RenderNode fixes

# 1.0.7
- Fix potential leak on orphaned RenderNodes

# 1.0.6
- Fixes for Better Touch Prio mod

# 1.0.5
- Fix a potential memory leak

# 1.0.4
- Remove cursor support for all platforms. Too annoying to deal with.

# 1.0.3
- Removing cursor support on MacOS for now. Restoring state is impossible currently.
- Fix some ScrollLayer bugs

# 1.0.2
- Reset cursor on close

# 1.0.1
- Fix blending bleeding into RoundedScrollHandle and RoundedScrollTrack
- Fix typo: `AdvancedScrollLayer::setVeritcalScroll` is now `AdvancedScrollLayer::setVerticalScroll`

# 1.0.1-beta.2
- Geode Alpha 2 support
- Switch to geode::NineSlice
- Disable touch blocker by default (touch prio is funky), it is easier to handle that on your own as a dev
- Bug Fixes

# 1.0.1-beta.1
- 2.2081 Support
- Fix minor bug when dragging with anything other than left click
- Temporarily disable DevTools support

# 1.0.0
- Initial Release
