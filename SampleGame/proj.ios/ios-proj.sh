# Replace cocos2d-x path with your environment relative path.

# It means cocos2dx root is ../../../../cocos2d-x-2.2.2
cocos_rpath=..\\/..\\/..\\/..\\/cocos2d-x-2.2.2

# It means cocos2dx root is ../../../../cocos2d-x-2.2.2
cocos_rpath_esc=\\.\\.\\/\\.\\.\\/\\.\\.\\/\\.\\.\\/cocos2d-x-2.2.2

if test "$1" = '-r' ; then
    echo "$1 option. revert local path related changes..."
    cat MyApp.xcodeproj/project.pbxproj | sed -e 's/'$cocos_rpath_esc'/__cocos2dx_relative_root__/g' > MyApp.xcodeproj/project.pbxproj.mod
else
    echo "Applying local path related changes..."
    cat MyApp.xcodeproj/project.pbxproj | sed -e 's/__cocos2dx_relative_root__/'$cocos_rpath'/g' > MyApp.xcodeproj/project.pbxproj.mod
fi

mv MyApp.xcodeproj/project.pbxproj.mod MyApp.xcodeproj/project.pbxproj
