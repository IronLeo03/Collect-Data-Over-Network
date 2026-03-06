// Setup type definitions for built-in Supabase Runtime APIs
import "jsr:@supabase/functions-js/edge-runtime.d.ts";
import { createClient } from 'jsr:@supabase/supabase-js@2'

Deno.serve(async (req) => {
  try {
    const supabase = createClient(
      Deno.env.get('SUPABASE_URL') ?? '',
      Deno.env.get('SUPABASE_SERVICE_ROLE_KEY') ?? ''
    )

    const { data, error } = await supabase.from('measurements').select('*')

    if (error) {
      throw error
    }

    // ${JSON.stringify(data)}

    const html = `<!doctype html>
<html>

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Measurements</title>

  <!-- https://getbootstrap.com/docs/5.3/getting-started/introduction/ -->
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css" rel="stylesheet"
    integrity="sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB" crossorigin="anonymous">
</head>

<body>
  <div class="container">
    <h1 class="text-center">Measurements</h1>
    <div class="d-flex flex-column gap-4 px-lg-5 py-2">
      <div class="card">
        <div class="card-body">
          <h6 class="card-subtitle text-center">Light</h6><canvas id="light_sensor"></canvas>
        </div>
      </div>
      <div class="card">
        <div class="card-body">
          <h6 class="card-subtitle text-center">Temperature</h6><canvas id="temperature_sensor"></canvas>
        </div>
      </div>
      <div class="card">
        <div class="card-body">
          <h6 class="card-subtitle text-center">Humidity</h6><canvas id="humidity_sensor"></canvas>
        </div>
      </div>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/js/bootstrap.bundle.min.js"
      integrity="sha384-FKyoEForCGlyvwx9Hj09JcYn3nv7wiPVlz7YYwJrWVcXK/BmnVDxM+D2scQbITxI"
      crossorigin="anonymous"></script>

    <script>
      const supabaseData = ${JSON.stringify(data)}

      let labels = []
      let light = []
      let temperature = []
      let humidity = []
      let ids = [] // Just to make sure I am doing everything in the right order

      for (const element of supabaseData) {
        ids.push(element["id"])
        labels.push(element["created_at"])
        light.push(element["measurements"]["light"])
        temperature.push(element["measurements"]["temperature"])
        humidity.push(element["measurements"]["humidity"])

        //console.log(element["id"])
      }

      function generateChart(id, label, data, color) {
        new Chart(document.getElementById(id), {
          type: 'line',
          options: {
            spanGaps: true,
            scales: { x: { ticks: { maxTicksLimit: 8 } } }
          },
          data: {
            labels: labels,
            datasets: [{
              label: label,
              data: data,
              fill: false,
              borderColor: color,
              pointRadius: 0
            }]
          }
        })
      }

      // https://www.w3schools.com/cssref/css_colors.php
      generateChart('light_sensor', 'Light', light, 'chocolate');
      generateChart('temperature_sensor', 'Temperature', temperature, 'crimson');
      generateChart('humidity_sensor', 'Humidity', humidity, 'darkblue');
    </script>
</body>

</html>`;

    return new Response(html, {
      headers: {
        'Content-Type': 'text/html; charset=utf-8',
        'Content-Disposition': `attachment; filename="measurement-report-${new Date().toISOString().split('T')[0]}.html"`,
      },
      status: 200,
    })
  } catch (err) {
    return new Response(JSON.stringify({ message: err?.message ?? err }), {
      headers: { 'Content-Type': 'application/json' },
      status: 500,
    })
  }
})
