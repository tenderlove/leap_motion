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

# Work around: https://github.com/seattlerb/hoe/issues/48
class Hoe
  alias :old_config :with_config

  def with_config
    old_config { |config, rc|
      patterns = [config['exclude'], /LeapSDK/, /bundle$/].compact
      config['exclude'] = Regexp.union(patterns)
      yield config, rc
    }
  end
end

hoe = Hoe.spec 'leap_motion' do
  developer('Aaron Patterson', 'aaron@tenderlovemaking.com')
  self.readme_file   = 'README.rdoc'
  self.history_file  = 'CHANGELOG.rdoc'
  self.extra_rdoc_files  = FileList['*.rdoc']

  extra_dev_deps << ['rake-compiler', '>= 0.4.1']

  self.spec_extras = {
    :extensions            => [ "ext/leap_motion/extconf.rb" ],
  }
end

task :relink do
  libleap = File.join SDK, 'lib', 'libLeap.dylib'
  sh "install_name_tool -change @loader_path/libLeap.dylib #{libleap} lib/leap_motion.bundle"
end

Rake::ExtensionTask.new "leap_motion", hoe.spec do |ext|
  ext.source_pattern = "*.{c,cpp}"
  ext.lib_dir = File.join(*['lib', ENV['FAT_DIR']].compact)
  ext.config_options << "--with-libLeap-dir=#{SDK}"
  ext.cross_compiling do |spec|
    spec.extensions << "ext/leap_motion/Rakefile"
  end
end

task build: [:clean, :compile, :relink]

# vim: syntax=ruby
