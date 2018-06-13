var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
	var keylist = Object.keys(global.ap);
	var list = [];
	for(i=0; i<keylist.length; i++ ){
		list[i] = global.ap[keylist[i]];
	}
  res.render('index', { 'list': list });
});

module.exports = router;
