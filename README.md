# QtHandlebarsJS

Qt binding to [Handlebars.js].


## What

QtHandlebarsJS is a Qt wrapper around [Handlebars.js]. It is built around QtQML's [QJSEngine], and targets modern Qt and C++.

Currently promised to work with Qt 5.4+. C++11 support is recommended.


## License

QtHandlebarsJS is released under the MIT license. You should find a copy of the license with the repository.

`handlebar-v3.0.3.js` is re-distributed under the MIT license.


## TODO

This project is at pre-alpha state. I plan to implement the followings before considering it "usable".

* `SafeString`
* Support `Handlebars.compile` options in `render`.
* Partials.
* Loggin support (bridge `Handlebars.log` to Qt's category logging system).


Some other features not considered necessary, but might be useful:

* `precompile`.
* `escapeExpression`. (Because we have `QString::toHtmlEscaped` already.)
* `Handlebars` instance copying/moving/assignment.


[Handlebars.js]: http://handlebarsjs.com
[QJSEngine]: http://doc.qt.io/qt-5/qjsengine.html
