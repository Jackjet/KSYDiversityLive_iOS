Pod::Spec.new do |s|
  s.name             = 'GPUImage'
  s.version          = '1.0.0'
  s.summary          = 'GPUImage from kiwiface'
  s.description      = <<-DESC
!!! for debug only !!!
* GPUImage frome kiwiface
                       DESC
  s.homepage     = 'http://newgit.op.ksyun.com/sdk/libksylivedep'
  s.author       = { 'PENGBIN' => 'pengbin@kingsoft.com' }
  s.license      = {
:type => 'Proprietary',
:text => <<-LICENSE
      Copyright 2017 kingsoft Ltd. All rights reserved.
      LICENSE
  }
  s.platform    = :ios, '8.0'
  s.ios.deployment_target = '8.0'
  s.requires_arc = true

  s.source      = { :git => 'git@newgit.op.ksyun.com:sdk/KSYLive_iOS.git', 
                    :tag => s.version.to_s }

  s.preserve_paths      = 'GPUImage.framework'
  s.public_header_files = 'GPUImage.framework/Headers'
  s.vendored_frameworks = 'GPUImage.framework'
end
