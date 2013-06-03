require 'rake/testtask'
require 'rake/clean'

NAME = "polarssl"
DLEXT = RbConfig::CONFIG['DLEXT']

file "lib/#{NAME}/#{NAME}.#{DLEXT}" => Dir.glob("ext/#{NAME}/*{.rb,.c}") do
  Dir.chdir("ext/#{NAME}") do
    ruby "extconf.rb"
    sh "make"
  end
  cp "ext/#{NAME}/#{NAME}.#{DLEXT}", "lib/#{NAME}"
end

task test: "lib/#{NAME}/#{NAME}.#{DLEXT}"

CLEAN.include("ext/**/*{.o,.log,.#{DLEXT}}")
CLEAN.include('ext/**/Makefile')
CLOBBER.include("lib/**/*.#{DLEXT}")

Rake::TestTask.new do |t|
  t.pattern = "test/*_test.rb"
end

task default: :test