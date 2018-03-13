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
  
  s.source_files = 'MapView/Map/*.{c,h,m}'
end
