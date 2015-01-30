im_sort
=======

in-memory sort replacement for sort command in linux systems

requirements
-----------
* boost_program_options
* scons
* g++

build
-----
scons is needed, so if it is installed run
> scons

after build process was successfully you can use im_sort, e.g.

> ./im_sort -h

    Allowed options:
     -h [ --help ]         produce help message
     -f [ --file ] arg     input file
     -o [ --output ] arg   output file

im_sort defaults behavior is to read from stdin and do the output to stdout.
It can be changed with the flags -f and -o to files.
Be sure that you have enough free memory (there are no checks).

benchmark
---------

so a few benchmark values tested on my system:

* Intel Core i5-3317U CPU @ 1.70GHz (4 logical cores)
* 8 GB DDR3 Ram
* files are stored on SSD
* test datasets from dbpedia (nt files)
    * (A): labels_en_uris_tr.nt (approx 17mb)
    * (B): labels_en_uris_de.nt (approx 97mb)
    * (C): labels_en (approx 1.4gb)
* command (for sort and im_sort):
> time cat dataset | SORT > OUT


<script type="text/javascript" src="https://www.google.com/jsapi"></script>
<script type="text/javascript">
  google.load("visualization", "1", {packages:["corechart"]});
  google.setOnLoadCallback(drawChart);
  function drawChart() {
    var data = google.visualization.arrayToDataTable([
      ['dataset', 'imsort', 'sort'],
      ['(A)', 1.310, 3.232],
      ['(B)', 7.753, 23.192],
      ['(C)', 106.99, 381.97]
    ]);

    var options = {
      title: 'benchmark, total runtime [seconds]'
    };

    var chart = new google.visualization.BarChart(document.getElementById('chart_div'));

    chart.draw(data, options);
  }


</script>
<div id="chart_div" style="width: 90%; height: 100px;"></div>


![Benchmark results](https://raw.githubusercontent.com/stg7/im_sort/master/bench.svg)
![Alt text](/bench.svg?raw=true "Optional Title")

* generally im_sort is quite faster than default sort
* default sort is using external sorting to handle large files, however it uses this algorithm in every case
* improvement ideas:
    * read file parallel to memory, so read time will be reduced