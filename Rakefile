require 'rake/testtask'
require 'rake/extensiontask'
require 'rake/clean'
require 'sdoc'

NAME = "polarssl"
DLEXT = RbConfig::CONFIG['DLEXT']

CLEAN.include("ext/**/*{.o,.log,.#{DLEXT}}")
CLEAN.include("ext/**/Makefile")
CLEAN.include("doc/**")
CLOBBER.include("lib/**/*.#{DLEXT}")

Rake::TestTask.new do |t|
  t.libs << "polarssl"
  t.pattern = "test/*_test.rb"
end

task test: :compile

RDOC_FILES = FileList["RDOC_MAIN.rdoc", "ext/polarssl/*.c", "lib/**/*.rb"]

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

task default: :test