var express = require('express');
var router = express.Router();

/* GET api listing. */
router.get('/version', function(req, res, next) {
  res.send({ 'version': '1.0.0' });
});

router.get('/echo', function(req, res, next) {
  res.send({ 'name': req.query.name });
});

// 新增接口，计算两数之和
router.get('/add', function(req, res, next) {
  // 获取传递的参数
  const num1 = parseInt(req.query.num1);
  const num2 = parseInt(req.query.num2);

  // 检查参数是否有效
  if (isNaN(num1) || isNaN(num2)) {
    res.status(400).send({ 'error': 'Invalid parameters. Please provide valid numbers.' });
  } else {
    // 计算两数之和并返回
    const sum = num1 + num2;
    res.send({ 'sum': sum });
  }
});

module.exports = router;
