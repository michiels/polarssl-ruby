require 'lib/polarssl/polarssl'

Gem::Specification.new do |s|
  s.name = 'polarssl'
  s.version = '0.0.1'
  s.date = '2013-08-20'
  s.summary = 'Use the PolarSSL cryptographic and SSL library in Ruby.'
  s.description = 'A gem that lets you use the PolarSSL cryptography library with Ruby.'
  s.authors = ['Michiel Sikkes']
  s.email = 'michiel.sikkes@gmail.com'
  s.files = `git ls-files`.split("\n")
  s.homepage = 'http://github.com/michiels/polarssl-ruby'
  s.license = 'LGPL-3'
  s.test_files = Dir.glob('test/*_test.rb')
  s.requirements = 'libpolarssl'

  s.extensions = %w[ext/polarssl/extconf.rb]
end