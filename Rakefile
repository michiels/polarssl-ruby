require 'rake/testtask'
require 'rake/clean'
require 'sdoc'
require 'grancher/task'

NAME = "polarssl"
DLEXT = RbConfig::CONFIG['DLEXT']

file "lib/#{NAME}/#{NAME}.#{DLEXT}" => Dir.glob("ext/#{NAME}/*{.rb,.c}") do
  Dir.chdir("ext/#{NAME}") do
    ruby "extconf.rb"
    sh "make"
  end
  cp "ext/#{NAME}/#{NAME}.#{DLEXT}", "lib/#{NAME}.#{DLEXT}"
end

task test: "lib/#{NAME}/#{NAME}.#{DLEXT}"

CLEAN.include("ext/**/*{.o,.log,.#{DLEXT}}")
CLEAN.include("ext/**/Makefile")
CLOBBER.include("lib/**/*.#{DLEXT}")

Rake::TestTask.new do |t|
  t.pattern = "test/*_test.rb"
end

RDOC_FILES = FileList["RDOC_MAIN.rdoc", "ext/polarssl/*.c"]

RDoc::Task.new do |rd|
  rd.rdoc_dir = "doc"
  rd.main = "RDOC_MAIN.rdoc"
  rd.title = "PolarSSL Ruby API"
  rd.rdoc_files.include(RDOC_FILES)
  rd.options << '-e' << 'UTF-8'
  rd.options << '-f' << 'sdoc'
  rd.options << '-T' << 'sdoc'
end

Grancher::Task.new do |g|
  g.branch = 'gh-pages'
  g.push_to 'origin'
  g.message = 'Updated website'
  g.directory 'doc', 'doc'
end

task default: :test