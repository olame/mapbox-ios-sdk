#
# Be sure to run `pod lib lint NAME.podspec' to ensure this is a
# valid spec and remove all comments before submitting the spec.
#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = "mapbox-ios-sdk"
  s.version          = "0.1"
  s.summary          = "Mapbox framework for iOS forked."
  s.homepage         = "https://github.com/olame/mapbox-ios-sdk"
  s.license          = 'MIT'
  s.author           = { "Mapbox" => "mobile@mapbox.com" }
  s.source           = { :git => "https://github.com/olame/mapbox-ios-sdk.git", :tag => s.version }
  s.social_media_url = 'https://twitter.com/mapbox'
  s.platform     = :ios, '5.0'
  s.source_files = 'Proj4/*.h', 'MapView/Map/*.{h,c,m}'
  s.requires_arc = true
  s.prefix_header_file = 'MapView/MapView_Prefix.pch'  
  s.post_install do |library|
    Dir.chdir File.join(library.sandbox_dir, 'MapBox') do
      command = "xcodebuild -project MapView/MapView.xcodeproj -target Resources CONFIGURATION_BUILD_DIR=../../Resources 2>&1 > /dev/null"
      unless system(command)
        raise ::Pod::Informative, "Failed to generate MapBox resources bundle"
      end
    end
    File.open(library.copy_resources_script_path, 'a') do |file|
      file.puts "install_resource 'Resources/mapbox-ios-sdk.bundle'"
    end
  end
  s.frameworks = 'CoreGraphics', 'CoreLocation', 'Foundation', 'QuartzCore', 'UIKit'

  s.libraries = 'Proj4', 'sqlite3', 'z'

  s.xcconfig = { 'OTHER_LDFLAGS' => '-ObjC', 'LIBRARY_SEARCH_PATHS' => '"${PODS_ROOT}/mapbox-ios-sdk/Proj4"' }

  s.preserve_paths = 'Proj4/libProj4.a', 'MapView/MapView.xcodeproj', 'MapView/Map/Resources'

  s.dependency 'FMDB @ 0cfca84'
  s.dependency 'GRMustache @ 6e51540'
  s.dependency 'SMCalloutView @ 19b961a'
end
