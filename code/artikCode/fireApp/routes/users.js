var express = require('express');
var router = express.Router();

/* GET users listing. */
router.get('/*', function(req,res,next){
        res.writeHead( 302, {
                'Location' : '/index'
        });
        res.end();
});

module.exports = router;
