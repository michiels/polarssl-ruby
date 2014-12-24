require 'rake/testtask'
require 'rake/extensiontask'
require 'rake/clean'
require 'sdoc'

NAME = "polarssl"
DLEXT = RbConfig::CONFIG['DLEXT']

CLEAN.include("ext/**/*{.o,.log,.#{DLEXT}}")
CLEAN.include("ext/**/Makefile")
CLEAN.include("doc")
CLOBBER.include("lib/**/*.#{DLEXT}")

Rake::TestTask.new do |t|
  t.pattern = "test/*_test.rb"
  t.libs << 'test'
end

task test: :compile

RDOC_FILES = FileList["RDOC_MAIN.rdoc", "lib/**/*.rb", "ext/polarssl/polarssl.c", "ext/polarssl/*.c"]

RDoc::Task.new do |rd|
  rd.rdoc_dir = "doc"
  rd.main = "RDOC_MAIN.rdoc"
  rd.title = "PolarSSL Ruby API"
  rd.rdoc_files.include(RDOC_FILES)
  rd.options << '-e' << 'UTF-8'
  rd.options << '-f' << 'sdoc'
  rd.options << '-T' << 'sdoc'
end

Rake::ExtensionTask.new "polarssl" do |ext|
  ext.lib_dir = "lib/polarssl"
end

desc "Signs the gem"
task :sign_gem do
  require 'digest/sha2'
  built_gem_path = "polarssl-#{ENV["VERSION"]}.gem"
  checksum = Digest::SHA512.new.hexdigest(File.read(built_gem_path))
  checksum_path = "checksum/polarssl-#{ENV["VERSION"]}.gem.sha512"
  File.open(checksum_path, 'w' ) {|f| f.write(checksum) }
end

task default: :test
