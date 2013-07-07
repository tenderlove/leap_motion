# -*- ruby -*-

require 'rubygems'
require 'hoe'

# Copy LeapSDK in to ext
SDK = File.join File.dirname(__FILE__), 'ext', 'LeapSDK'

Hoe.plugins.delete :rubyforge
Hoe.plugin :minitest
Hoe.plugin :gemspec # `gem install hoe-gemspec`
Hoe.plugin :git     # `gem install hoe-git`

gem 'rake-compiler', '>= 0.4.1'
require "rake/extensiontask"

hoe = Hoe.spec 'leap_motion' do
  developer('Aaron Patterson', 'aaron@tenderlovemaking.com')
  self.readme_file   = 'README.rdoc'
  self.history_file  = 'CHANGELOG.rdoc'
  self.extra_rdoc_files  = FileList['*.rdoc']

  extra_dev_deps << ['rake-compiler', '>= 0.4.1']

  self.spec_extras = {
    :extensions            => ["ext/leap_motion/extconf.rb"],
  }
end

ENV['DYLD_LIBRARY_PATH'] = File.join SDK, 'lib'

Rake::ExtensionTask.new "leap_motion", hoe.spec do |ext|
  ext.lib_dir = File.join(*['lib', ENV['FAT_DIR']].compact)
  ext.config_options << "--with-libLeap-dir=#{SDK}"
end

# vim: syntax=ruby
