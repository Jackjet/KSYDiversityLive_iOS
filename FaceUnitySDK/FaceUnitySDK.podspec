Pod::Spec.new do |s|
  s.name         = 'FaceUnitySDK'
  s.version      = '1.0.0'
  s.license      = {
:type => 'Proprietary',
:text => <<-LICENSE
      Copyright 2015 kingsoft Ltd. All rights reserved.
      LICENSE
  }
  s.homepage     = 'http://v.ksyun.com/doc.html'
  s.authors      = { 'ksyun' => 'zengfanping@kingsoft.com' }
  s.summary      = 'add filter for player'
  s.description  = <<-DESC
    * KSYPlayerDep  ksyplayer dependencies
    * FaceUnitySDK  FaceUnity SDK
  DESC
  s.platform     = :ios, '7.0'
  s.ios.library = 'z', 'iconv', 'stdc++.6', 'bz2'
  s.ios.deployment_target = '7.0'
  s.source = { 
    :git => 'https://github.com/ksvc/KSYDiversityLive_iOS',
    :tag => 'v'+s.version.to_s
  }
  s.requires_arc = true
  s.pod_target_xcconfig = { 'OTHER_LDFLAGS' => '-lObjC -all_load' }

  s.subspec 'KSYPlayerDep' do |sub|
    sub.source_files = [ 'prebuilt/include/**/*.h',
                         'source/KSYFaceUnityWrapper.m',
                         'source/KSYFaceUnityWrapper.h']
    sub.vendored_library = 'prebuilt/libs/libksygpuimageext.a'
  end
  # FaceUnity Sticker
  s.subspec 'FaceUnitySDK' do |sub|
    sub.resource = 'framework/*.bundle'
    sub.source_files = 'framework/*.h'
    sub.vendored_library = 'framework/libnama.a'
  end
end
