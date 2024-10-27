using Newtonsoft.Json;

public class LeiturasModel
{
    /// <summary>
    /// Lista de propriedades dos gr�ficos
    /// </summary>
    [JsonProperty("charts")]
    public Chart Charts { get; set; }

    /// <summary>
    /// Lista das leituras
    /// </summary>
    [JsonProperty("readings")]
    public List<Data> Readings { get; set; }


    public class Chart
    {
        /// <summary>
        /// N�mero de leituras exibidas no gr�fico
        /// </summary>
        [JsonProperty("range")]
        public string Range { get; set; }
    }

    public class Data
    {
        /// <summary>
        /// Valor da leitura, em mV, da rota FirstEmg que contempla o m�sculo flexor
        /// </summary>
        [JsonProperty("FirstEmg")]
        public string FirstEmg { get; set; }

        /// <summary>
        /// Hor�rio da leitura
        /// </summary>
        [JsonProperty("timestamp")]
        public string Timestamp { get; set; }
    }
}