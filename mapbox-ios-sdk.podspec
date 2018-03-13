#
# Be sure to run `pod lib lint NAME.podspec' to ensure this is a
# valid spec and remove all comments before submitting the spec.
#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#
Pod::Spec.new do |s|
  s.author = { "Mapbox" => "mobile@mapbox.com" }
  s.homepage = "https://www.mapbox.com/mapbox-ios-sdk/"
  s.source = { :git => "https://github.com/mapbox/mapbox-ios-sdk.git", :tag => "v#{s.version.to_s}" }
  s.summary = "Mapbox framework for iOS forked."
  s.name             = "mapbox-ios-sdk"
  s.version          = '0.1'
  s.platform     = :ios, '5.0'
  s.ios.deployment_target = "5.0"
  s.requires_arc = true
  s.source_files = 'mapbox-ios-sdk','mapbox-ios-sdk/**/*.{swift,h,m}'

end
