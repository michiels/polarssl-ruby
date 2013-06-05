polarssl-ruby
=============

Use PolarSSL in your Rubies. Ruby wrapper for the PolarSSL cryptographic and SSL/TLS library.

Development
===========

Install PolarSSL from source via https://polarssl.org/download or install it using your operating system. For example:

On Ubuntu:

```
sudo apt-get install libpolarssl-dev
```

On Mac OS X with [Homebrew](http://mxcl.github.io/homebrew/):

```
brew install polarssl
```

The following steps and commands are followed during development:

1. A branch is created.
2. Tests are created in `test/` before code is written and ran with `rake test`. This rake task takes care of compiling the binary and executing the tests.
3. Code is written.
4. A pull request is created.
5. CI runs and verifies passing tests of the Pull Request.
6. The pull request is merged in.
5. Repeat for every feature change or bug fix.

Tools used when developing:

* Vagrant CI (http://vagrant-ci.org)
* MiniTest (built into Ruby)
* GitHub

License
=======

```
polar-ssl-ruby - A Ruby extension for using PolarSSL.
Copyright (C) 2013  Michiel Sikkes

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```